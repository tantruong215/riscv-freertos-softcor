    li   t0, 7
    beq  a1, t0, handle_mtimer
    ...
handle_mtimer:
    call xPortSysTickHandler
    mret
