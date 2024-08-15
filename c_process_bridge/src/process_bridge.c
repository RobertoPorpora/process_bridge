#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include "process_bridge.h"

#ifdef _WIN32
const char *NEWLINE = "\r\n";
#else
const char *NEWLINE = "\n";
#endif

// -----------------------------------------------------------------------------
// Create / Destroy functions
// -----------------------------------------------------------------------------

PB_child_t *PB_child_create(void)
{
    PB_child_t *child = (PB_child_t *)malloc(sizeof(PB_child_t));
    if (child == NULL)
    {
        return NULL;
    }
    child->status = PB_STATUS_NOT_SPAWNED;
    strcpy(child->error, "Child not spawned");
    return child;
}

void PB_child_destroy(PB_child_t *handle)
{
    if (handle != NULL)
    {
        free(handle);
    }
}

PB_parent_bridge_t *PB_parent_bridge_create(void)
{
    PB_parent_bridge_t *bridge = (PB_parent_bridge_t *)malloc(sizeof(PB_parent_bridge_t));
    if (bridge == NULL)
    {
        return NULL;
    }
    bridge->status = PB_STATUS_NOT_INIT;
    strcpy(bridge->error, "Parent bridge not initialized");
    return bridge;
}

void PB_parent_bridge_destroy(PB_parent_bridge_t *handle)
{
    if (handle != NULL)
    {
        free(handle);
    }
}

// -----------------------------------------------------------------------------
// PARENT_SIDE - Child process life management
// -----------------------------------------------------------------------------

PB_status_t PB_spawn(PB_child_t *child, const char *command)
{
    strcpy(child->error, "Not implemented.");
    child->status = PB_STATUS_GENERIC_ERROR;
    return PB_STATUS_GENERIC_ERROR;
}

PB_status_t PB_despawn(PB_child_t *child)
{
    strcpy(child->error, "Not implemented.");
    child->status = PB_STATUS_GENERIC_ERROR;
    return PB_STATUS_GENERIC_ERROR;
}

PB_status_t PB_wait(PB_child_t *child)
{
    strcpy(child->error, "Not implemented.");
    child->status = PB_STATUS_GENERIC_ERROR;
    return PB_STATUS_GENERIC_ERROR;
}

PB_return_t PB_return_code(PB_child_t *child)
{
    strcpy(child->error, "Not implemented.");
    child->status = PB_STATUS_GENERIC_ERROR;
    return PB_STATUS_GENERIC_ERROR;
}

// -----------------------------------------------------------------------------
// PARENT_SIDE - Child process communications
// -----------------------------------------------------------------------------

PB_status_t PB_p2c_send(PB_child_t *child, const char *message)
{
    strcpy(child->error, "Not implemented.");
    child->status = PB_STATUS_GENERIC_ERROR;
    return PB_STATUS_GENERIC_ERROR;
}

PB_status_t PB_p2c_receive(PB_child_t *child, char *message, size_t size)
{
    strcpy(child->error, "Not implemented.");
    child->status = PB_STATUS_GENERIC_ERROR;
    return PB_STATUS_GENERIC_ERROR;
}

PB_status_t PB_p2c_receive_err(PB_child_t *child, char *message, size_t size)
{
    strcpy(child->error, "Not implemented.");
    child->status = PB_STATUS_GENERIC_ERROR;
    return PB_STATUS_GENERIC_ERROR;
}

// -----------------------------------------------------------------------------
// PARENT_SIDE - Child status check
// -----------------------------------------------------------------------------

PB_status_t PB_get_child_status(PB_child_t *child)
{
    return child->status;
}

char *PB_get_child_error(PB_child_t *child, char *error)
{
    if (error != NULL)
    {
        strcpy(error, child->error);
    }
    return child->error;
}

// -----------------------------------------------------------------------------
// CHILD_SIDE - Parent status check
// -----------------------------------------------------------------------------

PB_status_t PB_get_parent_bridge_status(PB_parent_bridge_t *bridge)
{
    return bridge->status;
}

char *PB_get_parent_bridge_error(PB_parent_bridge_t *bridge, char *error)
{
    if (error != NULL)
    {
        strcpy(error, bridge->error);
    }
    return bridge->error;
}

// -----------------------------------------------------------------------------
// CHILD_SIDE - Parent process communication
// -----------------------------------------------------------------------------

PB_status_t PB_parent_bridge_init(PB_parent_bridge_t *bridge)
{
    if (bridge == NULL)
    {
        strcpy(bridge->error, "Bridge argument is NULL.");
        bridge->status = PB_STATUS_GENERIC_ERROR;
        return bridge->status;
    }
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    strcpy(bridge->error, "");
    bridge->status = PB_STATUS_OK;
    return bridge->status;
}

static PB_status_t c2p_send_common(PB_parent_bridge_t *bridge, const char *message, bool is_err)
{
    if (bridge == NULL)
    {
        strcpy(bridge->error, "Bridge argument is NULL");
        bridge->status = PB_STATUS_GENERIC_ERROR;
        return PB_STATUS_GENERIC_ERROR;
    }

    if (message == NULL)
    {
        strcpy(bridge->error, "Message argument is NULL");
        bridge->status = PB_STATUS_GENERIC_ERROR;
        return PB_STATUS_GENERIC_ERROR;
    }

    int result;
    if (is_err)
    {
        result = fprintf(stderr, "%s%s", message, NEWLINE);
    }
    else
    {
        result = fprintf(stdout, "%s%s", message, NEWLINE);
    }

    if (result < 0)
    {
        strcpy(bridge->error, strerror(errno));
        bridge->status = PB_STATUS_GENERIC_ERROR;
        return PB_STATUS_GENERIC_ERROR;
    }

    strcpy(bridge->error, "");
    bridge->status = PB_STATUS_OK;
    return PB_STATUS_OK;
}

PB_status_t PB_c2p_send(PB_parent_bridge_t *bridge, const char *message)
{
    return c2p_send_common(bridge, message, false);
}

PB_status_t PB_c2p_send_err(PB_parent_bridge_t *bridge, const char *message)
{
    return c2p_send_common(bridge, message, true);
}

PB_status_t PB_c2p_receive(PB_parent_bridge_t *bridge, char *message, size_t size)
{
    if (bridge == NULL)
    {
        strcpy(bridge->error, "Bridge argument is NULL");
        bridge->status = PB_STATUS_GENERIC_ERROR;
        return PB_STATUS_GENERIC_ERROR;
    }

    if (message == NULL)
    {
        strcpy(bridge->error, "Message argument is NULL");
        bridge->status = PB_STATUS_GENERIC_ERROR;
        return PB_STATUS_GENERIC_ERROR;
    }

    if (fgets(message, size, stdin) == NULL)
    {
        if (feof(stdin))
        {
            strcpy(bridge->error, "End of file reached");
        }
        else if (ferror(stdin))
        {
            strcpy(bridge->error, strerror(errno));
        }
        else
        {
            strcpy(bridge->error, "Unknown error");
        }
        bridge->status = PB_STATUS_GENERIC_ERROR;
        return PB_STATUS_GENERIC_ERROR;
    }

    // Rimuove il carattere di nuova riga se presente
    size_t len = strlen(message);
    if (len > 0)
    {
        if (message[len - 1] == '\n')
        {
            message[len - 1] = '\0'; // expression must be a modifiable lvalue
        }
        else if (len > 1 && message[len - 2] == '\r' && message[len - 1] == '\n')
        {
            message[len - 2] = '\0';
        }
    }

    strcpy(bridge->error, "");
    bridge->status = PB_STATUS_OK;
    return PB_STATUS_OK;
}