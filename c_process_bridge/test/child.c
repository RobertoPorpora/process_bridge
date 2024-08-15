#include <process_bridge.h>
#include <string.h>
#include <stdio.h>
void sleep_millis(int millis);

int main(void)
{
    PB_parent_bridge_t *parent = PB_parent_bridge_create();
    PB_parent_bridge_init(parent);
    char buf[3][10];
    memset(buf, 0, sizeof(buf));
    PB_c2p_receive(parent, buf[0], 10);
    PB_c2p_receive(parent, buf[1], 10);
    PB_c2p_receive(parent, buf[2], 10);

    char out[200];
    sprintf(out, "c1 %s %s %s", buf[0], buf[1], buf[2]);
    PB_c2p_send(parent, out);
    PB_c2p_send_err(parent, out);
    PB_c2p_send(parent, "c2");
    PB_c2p_send_err(parent, "c2");
    PB_c2p_send(parent, "c3");
    PB_c2p_send_err(parent, "c3");

    sleep_millis(1000);

    return 12;
}

#ifdef _WIN32
#include <windows.h>
void sleep_millis(int millis)
{
    Sleep(millis);
}
#else
#include <unistd.h>
void sleep_millis(int millis)
{
    usleep(millis * 1000);
}
#endif