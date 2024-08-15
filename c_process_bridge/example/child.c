#include <process_bridge.h>
#include <string.h>
#include <stdio.h>
void sleep_millis(int millis);

int main(void)
{
    PB_parent_bridge_t *parent = PB_parent_bridge_create();
    PB_parent_bridge_init(parent);
    sleep_millis(300);
    PB_c2p_send(parent, "CHILD: Message sent to stdout after a short delay.");
    sleep_millis(400);
    PB_c2p_send_err(parent, "CHILD: Error message sent to stderr after a longer delay.");
    char buffer[200];
    size_t index;
    sprintf(buffer, "CHILD: Received data: ");
    index = strlen(buffer);
    PB_c2p_receive(parent, &buffer[index], 100);
    PB_c2p_send(parent, buffer);
    PB_c2p_send(parent, "CHILD: Exiting with code 12.");
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