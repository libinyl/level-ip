#include "basic.h"
#include "syshead.h"
#include "utils.h"

/**
 * 参考: https://www.cnblogs.com/charlieroro/p/13497340.html
 *      https://www.kernel.org/doc/Documentation/networking/tuntap.rst
 *      https://www.ibm.com/developerworks/cn/linux/l-tuntap/?ca=dwcn-newsletter-linux#ibm-pcon
 *
 */

static int tun_fd;
static char *dev;

char *tapaddr = "10.0.0.5";
char *taproute = "10.0.0.0/24";

/**
 * https://phoenixnap.com/kb/linux-ip-command-examples
 */
static int set_if_route(char *dev, char *cidr) { return run_cmd("ip route add dev %s %s", dev, cidr); }

static int set_if_address(char *dev, char *cidr) { return run_cmd("ip address add dev %s local %s", dev, cidr); }

static int set_if_up(char *dev) { return run_cmd("ip link set dev %s up", dev); }

/*
 * 初始化 tun 网卡设备
 *
 * Taken from Kernel Documentation/networking/tuntap.txt
 *
 *
 * dev: 网卡名称
 */
static int tun_alloc(char *dev) {
    struct ifreq ifr;
    int fd, err;

    /**
     * 1. 以读写权限打开 tap 设备, 获得代表它的 fd. //todo 为什么必须是读写权限?
     * 此过程将初始化一些重要的数据结构
     */
    if ((fd = open("/dev/net/tap", O_RDWR)) < 0) {
        perror("Cannot open TUN/TAP dev\n"
               "Make sure one exists with "
               "'$ mknod /dev/net/tap c 10 200'");
        exit(1);
    }

    CLEAR(ifr);

    /*
     * 如果指定了设备名称, 那么把它放入 ifr.ifr_name
     * 如果没有指定, the kernel will try to allocate the "next" device of the specified type // todo ?
     *
     * Flags: IFF_TUN   - TUN device 无以太头(Ethernet headers)
     *        IFF_TAP   - TAP device 有以太头
     *
     *        IFF_NO_PI - 告诉内核不要提供报文信息, 即告诉内核仅需要提供"纯"ip 报文, 不需要其他字节.
     *                    如果不设置此选项, 会在报文开始处添加4个额外的字节(2字节的标识和2字节的协议)
     */
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    if (*dev) {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    /**
     * 把 fd 注册到内核中
     *
     */
    if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0) {
        perror("ERR: Could not ioctl tun");
        close(fd);
        return err;
    }

    /**
     * 把创建成功的网卡名称回写到 dev
     */
    strcpy(dev, ifr.ifr_name);
    return fd;
}

int tun_read(char *buf, int len) { return read(tun_fd, buf, len); }

int tun_write(char *buf, int len) { return write(tun_fd, buf, len); }

void tun_init() {
    dev = calloc(10, 1);
    tun_fd = tun_alloc(dev); // 注册

    if (set_if_up(dev) != 0) { // 使能
        print_err("ERROR when setting up if\n");
    }

    if (set_if_route(dev, taproute) != 0) { // 设置路由
        print_err("ERROR when setting route for if\n");
    }

    if (set_if_address(dev, tapaddr) != 0) { // 设置地址
        print_err("ERROR when setting addr for if\n");
    }
}

void free_tun() { free(dev); }
