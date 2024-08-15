#pragma once

#include <stdint.h>
#include <stddef.h>

// -----------------------------------------------------------------------------
// Types
// -----------------------------------------------------------------------------

typedef enum
{
    PB_STATUS_OK = 0,
    PB_STATUS_NOT_INIT,
    PB_STATUS_NOT_SPAWNED,
    PB_STATUS_COMPLETED,
    PB_STATUS_TERMINATED,
    PB_STATUS_GENERIC_ERROR,
} PB_status_t;

typedef uint8_t PB_return_t;

typedef struct PB_child_t
{
    PB_status_t status;
    char error[200];
    PB_return_t return_code;
} PB_child_t;

typedef struct PB_parent_bridge_t
{
    PB_status_t status;
    char error[200];
} PB_parent_bridge_t;

// -----------------------------------------------------------------------------
// Create / Destroy functions
// -----------------------------------------------------------------------------

PB_child_t *PB_child_create(void);
void PB_child_destroy(PB_child_t *);
PB_parent_bridge_t *PB_parent_bridge_create(void);
void PB_parent_bridge_destroy(PB_parent_bridge_t *);

// -----------------------------------------------------------------------------
// PARENT_SIDE - Child process life management
// -----------------------------------------------------------------------------

// NOT IMPLEMENTED!
PB_status_t PB_spawn(PB_child_t *, const char *);
// NOT IMPLEMENTED!
PB_status_t PB_despawn(PB_child_t *);
// NOT IMPLEMENTED!
PB_status_t PB_wait(PB_child_t *);
// NOT IMPLEMENTED!
PB_return_t PB_return_code(PB_child_t *);

// -----------------------------------------------------------------------------
// PARENT_SIDE - Child process communications
// -----------------------------------------------------------------------------

// NOT IMPLEMENTED!
PB_status_t PB_p2c_send(PB_child_t *, const char *);
// NOT IMPLEMENTED!
PB_status_t PB_p2c_receive(PB_child_t *, char *, size_t);
// NOT IMPLEMENTED!
PB_status_t PB_p2c_receive_err(PB_child_t *, char *, size_t);

// -----------------------------------------------------------------------------
// PARENT_SIDE - Child status check
// -----------------------------------------------------------------------------

PB_status_t PB_get_child_status(PB_child_t *);
char *PB_get_child_error(PB_child_t *, char *);

// -----------------------------------------------------------------------------
// CHILD_SIDE - Parent status check
// -----------------------------------------------------------------------------

PB_status_t PB_get_parent_bridge_status(PB_parent_bridge_t *);
char *PB_get_parent_bridge_error(PB_parent_bridge_t *, char *);

// -----------------------------------------------------------------------------
// CHILD_SIDE - Parent process communication
// -----------------------------------------------------------------------------

PB_status_t PB_parent_bridge_init(PB_parent_bridge_t *);
PB_status_t PB_c2p_send(PB_parent_bridge_t *, const char *);
PB_status_t PB_c2p_send_err(PB_parent_bridge_t *, const char *);
PB_status_t PB_c2p_receive(PB_parent_bridge_t *, char *, size_t);