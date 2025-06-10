# scripts/patch_addresses.ps1
# Reads LED_ADDR and UART_ADDR from top.v, patches src/led.c & src/uart.c, then rebuilds & simulates.

# 1) Read the memory?map from top.v
$top = Get-Content .\top.v

# Extract the LED and UART base addresses
$ledAddr = ($top |
    Select-String -Pattern 'LED_ADDR\s*=\s*32\'h([0-9A-Fa-f_]+)' |
    ForEach-Object { $_.Matches[0].Groups[1].Value -replace '_','' }
)
$uartAddr = ($top |
    Select-String -Pattern 'UART_ADDR\s*=\s*32\'h([0-9A-Fa-f_]+)' |
    ForEach-Object { $_.Matches[0].Groups[1].Value -replace '_','' }
)

# Prepend ?0x?
$ledAddr  = "0x$ledAddr"
$uartAddr = "0x$uartAddr"

Write-Host "Detected LED_ADDR = $ledAddr"
Write-Host "Detected UART_ADDR = $uartAddr"

# 2) Patch src/led.c
(Get-Content .\src\led.c) `
  -replace '#define LED_REG\s+\(\*volatile uint32_t\*\)\s*0x[0-9A-Fa-f]+' , "#define LED_REG (*(volatile uint32_t*)$ledAddr)" `
  | Set-Content .\src\led.c

# 3) Patch src/uart.c
(Get-Content .\src\uart.c) `
  -replace '#define UART_BASE\s*0x[0-9A-Fa-f]+UL', "#define UART_BASE $uartAddr`UL" `
  | Set-Content .\src\uart.c

Write-Host "Patched C drivers with new base addresses."

# 4) Rebuild & simulate
& make clean
& make sim
