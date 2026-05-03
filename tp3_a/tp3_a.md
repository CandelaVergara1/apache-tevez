# TP3 — Arquitectura y Fundamentos del Firmware UEFI

### Grupo: Apache Tevez

### Profesores:

- Miguel Angel Solinas

- Javier Jorge

## Integrantes

| Nombre                            | Correo Electrónico                |
| --------------------------------- | --------------------------------- |
| Facundo Emanuel Avila Diaz Moreno | facundo.avila.027@mi.unc.edu.ar   |
| Candela Abigail Vergara           | candela.vergara@mi.unc.edu.ar     |
| Joaquín Alejandro Salinas         | joaquin.salinas.874@mi.unc.edu.ar |



## Trabajo Práctico 2: Desarrollo, compilación y análisis de seguridad

### 2.1 Desarrollo de la aplicación

Se utilizó el código fuente provisto en la guía (`aplicación.c`), que implementa una aplicación UEFI nativa en C. La aplicación recibe como párametros el `ImageHandle` y un puntero a la `EFI_SYSTEM_TABLE`, incializa la librería con `InitializeLib()`, y utiliza `SystemTable->ConOut->OutputString` para imprimir dos mensajes en la consola del firmware. Además, declara un array con el byte `0xCC` (opcode de la instrucción `INT3`) y verifica su valor mediante una condición `if`.

```C
#include <efi.h>
#include <efilib.h>
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
InitializeLib(ImageHandle, SystemTable);
SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Iniciando analisis de seguridad...\r\n");
// Inyección de un software breakpoint (INT3)
unsigned char code[] = { 0xCC };
if (code[0] == 0xCC) {
SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Breakpoint estatico alcanzado.\r\n");
}
return EFI_SUCCESS;
}

```

#### ¿Por qué utilizamos `SystemTable->ConOut->OutputString` en lugar de la función `printf` de C?

Se utiliza `SystemTable->ConOut->OutputString` porque no hay sistema operativo cargado, por lo tanto no existe la librería estándar de C. `printf` depende de libc, que necesita un OS para gestionar stdout y syscalls. En UEFI, el firmware expone sus servicios a través de la System Table: `ConOut` es el protocolo SIMPLE_TEXT_OUTPUT y `OutputString` es la función que escribe directamente en pantalla a través del firmware.

### 2.2 Compilación a formato PE/COFF

El proceso de compilación se realizó en tres etapas desde Linux. Primero se compiló el código fuente a un archivo de objeto (`.o`) con `gcc` usando flags específicas para un entorno freestanding (`ffreestanding`, `-fno-stack-protector`,`-mno-red-zone`), ya que no existe libc ni sistema operativo. Luego se linkeó el objeto a un `.so` intermedio en formato ELF utilizando `ld` con el linker script `elf_x86_64_efi.lds` y el startup object `crt0-efi-x86_64.o` provistos por `gnu-efi`. Finalmente, se convirtió el ELF a formato PE/COFF mediante `objcopy` con el target `efi-app-x86_64`, generando el ejecutable `aplicacion.efi`.
Se verificó el resultado con `file aplicacion.efi`, que confirmó el formato `PE32+executable (EFI application)`. Al ejecutar `readelf -h aplicacion.efi` se obtuvo un error ("Not an ELF file"), lo cual es esperado: el archivo ya no es ELF (formato de Linux) sino PE/COFF (formato requerido por la especificación UEFI), confirmando que la conversión fue exitosa.

### 2.3 Análisis de metadatos y compilación

Se importó `aplicacion.efi` en Ghidra para analizar el binario a nivel de pseudocódigo. Al decompilar la función `efi_main`, se observó que Ghidra representa las llamadas a `OutputString` como operaciones con punteros crudos y offsets de memoria (por ejemplo, `(*(longlong *)(unaff_RSI + 0x40) + 8)`), ya que el decompilador no posee las definiciones de tipos de UEFI como `EFI_SYSTEM_TABLE` o `SIMPLE_TEXT_OUTPUT_PROTOCOL`. El registro `unaff_RSI` corresponde al puntero a la System Table que recibe la función como parámetro. También se observó que el compilador optimizó la condición `if (code[0] == 0xCC)`, eliminándola del binario al detectar que siempre es verdadera.

![](img/1.jpeg)

**Figura 1:** Vista del CodeBrowser de Ghidra con la función efi_main seleccionada. A la izquierda se observa el Symbol Tree con las funciones del binario, en el centro el Listing con el código ensamblador x86-64, y a la derecha el panel Decompiler con el pseudocódigo generado.

![](img/2.jpeg)
**Figura 2:** Pseudocódigo decompilado de efi_main por Ghidra. Se observa que las llamadas a OutputString se representan como punteros crudos con offsets (unaff_RSI + 0x40), ya que Ghidra no posee las definiciones de tipos de UEFI. También se evidencia que el compilador eliminó la condición if (code[0] == 0xCC) al detectar que siempre era verdadera, ejecutando ambos OutputString de forma secuencial.

####  En el pseudocódigo de Ghidra, la condición 0xCC suele aparecer como -52. ¿A qué se debe este fenómeno y por qué importa en ciberseguridad?

En el pseudocódigo de Ghidra, `0xCC` aparece como `-52` porque el decompilador interpreta el byte como `signed char` en lugar de `unsigned char`. El valor `0xCC` en binario es `11001100`; como unsigned equivale a 204, pero como signed, al tener el bit más significativo en 1, se interpreta en complemento a 2: 256 - 204 = 52, resultando en `-52`. Son los mismos bits con distinta interpretación de signo. Esto es relevante en ciberseguridad porque `0xCC` es el opcode de la instrucción `INT3` (software breakpoint en x86), utilizado tanto por debuggers como por malware como técnica anti-debugging. Si un analista no reconoce que `-52` en el pseudocódigo es en realidad `0xCC/INT3`, podría pasar por alto una instrucción de breakpoint inyectada en un binario malicioso.