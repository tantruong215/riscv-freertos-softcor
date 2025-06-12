# scripts/patch_addresses.ps1
# Reads LED_BASE_ADDR and UART_BASE_ADDR from src/verilog/top.v
# and patches them into src/c/led.h and src/c/uart.h

# Load top.v
$top = Get-Content "../src/verilog/top.v"

# Extract LED address (search for localparam LED_ADDR = 32'h...)
$ledAddr = ($top |
    Select-String -Pattern "localparam\s+LED_ADDR\s*=\s*32'h([0-9A-Fa-f_]+)" |
    ForEach-Object { $_.Matches[0].Groups[1].Value -replace '_','' }
)

# Extract UART address
$uartAddr = ($top |
    Select-String -Pattern "localparam\s+UART_ADDR\s*=\s*32'h([0-9A-Fa-f_]+)" |
    ForEach-Object { $_.Matches[0].Groups[1].Value -replace '_','' }
)

if (-not $ledAddr -or -not $uartAddr) {
    Write-Error "Failed to parse LED_ADDR or UART_ADDR in top.v"
    exit 1
}

# Patch LED header
(Get-Content "../src/c/led.h") `
  -replace '#define\s+LED_BASE_ADDR\s+0x[0-9A-Fa-f]+' , "#define LED_BASE_ADDR   0x$ledAddr`U" `
  | Set-Content "../src/c/led.h"

# Patch UART header
(Get-Content "../src/c/uart.h") `
  -replace '#define\s+UART_BASE_ADDR\s+0x[0-9A-Fa-f]+' , "#define UART_BASE_ADDR  0x$uartAddr`U" `
  | Set-Content "../src/c/uart.h"

Write-Host "✅ Patched LED_BASE_ADDR and UART_BASE_ADDR in headers."
