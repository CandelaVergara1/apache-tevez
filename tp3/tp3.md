# TP2 — Modo protegido

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
---


## UEFI y coreboot

#### 1. ¿Qué es UEFI? ¿como puedo usarlo? Mencionar además una función a la que podría llamar usando esa dinámica.

UEFI (Unified Extensible Firmware Interface) es el reemplazo moderno del BIOS. En vez de estar limitado a modo real de 16 bits con 1 MB de memoria y acceso por interrupciones, UEFI arranca en 32 o 64 bits, ve toda la memoria, soporta discos grandes con GPT, y ofrece una API moderna con funciones llamables en vez de interrupciones.

Para usarlo se puede escribir aplicaciones UEFI, las cuales son ejecutables en formato PE que el firmware carga directamente, sin necesidad de MBR ni bootloader. El flujo sería asi:
1. Se escribe un programa en C usando las cabeceraas del estándar UEFI (o con herramientas como `gnu-efi` o el `EDK2` de Intel/Tianocore).

2. La función de entrada recibe dos parámetros: un `ImageHandle` y un puntero a la `SystemTable`, que es la tabla raíz con punteros a todos los servicios (Boot services, Runtime Services, protocolos de I/O,etc).

3. Se compila y se deja una partición EFI (FAT32) del disco.

4. El firmware UEFI lo encuentra y lo ejecuta directamente, ya en modo protegido.

Una función de ejemplo usando esta dinámica:
`SystemTable->ConOut->OutputString(ConOut, L"Hola Mundo")` - esto imprime texto en pantalla usando el protocolo `EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL`. A diferencia del BIOS donde se hace `int 0x10` con parámetros en registros, aca se llama una función directamente a través de la tabla de servicios.

#### 2. ¿Menciona casos de bugs de UEFI que puedan ser explotados?

- LoJax (2018): fue el primer rootkit UEFI encontrado en uso real, creado por el grupo Sednit. Modificaba el firmware UEFI para instalar malware que sobrevivía incluso a reinstalaciones del sistema operativo y cambios de disco.
- Blacklotus (2023): Fue el primer bootkit UEFI capaz de evadir Secure Boot. Explotaba vulnerabilidades para ejecutar código malicioso antes de que arranque el sistema operativo, afectando a organizaciones gubernamentales y empresas. Básicamente insertaba un bootloader falso en la partición EFI que el firmware cargaba porque la base de datos de revoación (DB/DBX) no estaba correctamente configurada.
- PixieFail (2024): Fueron múltiples vulnerabilidades descubiertas en el stack de red del TianoCore EDK II (la implementación open-source de UEFI que usan la mayoría de fabricantes). Podían ser explotadas para lograr ejecución remota de código, denegación de servicio y envenenamiento de caché DNS.
- Vulnerabilidades en InsydeH20: investigadores de Binarly encontraron 23 fallas en el firmware UEFI de InsydeH20, la mayoría en el System Management Mode (SMM). Un atacante explotando estas fallas podía invalidar funciones de seguridad como Secure Boot,instalar software persistente, y crear backdoors para robar datos. Estas vulnerabilidades afectaban a más de 25 fabricantes, incluyendo Fujitsu, Intel, AMD, Lenovo, Dell, Asus, HP y Microsoft.

Lo que tienen en común todos estos bugs es que al estar en el firmware (debajo del sistema operativo), el antivirus no los detecta y sobreviven a reinstalaciones del SO. 


#### 3. ¿Qué es Converged Security and Management Engine (CSME), the Intel Management Engine BIOS Extension (Intel MEBx).?
  
- Intel Management Engine (ME) es un subsistema que funciona como una computadora dentro de una computadora. Es un microprocesador independiente integrado en el chipsett de Intel que corre su propio sistema operativo (MINIX 3 a partir de la version 11) y tiene acceso directo al hardware: memoria, red, almacenamiento. Funciona incluso cuando la computadora está apagada (siempre que tenga energía conectada), y el sistema operativo principal no puede verlo ni controlarlo.
- Intel MEBx (Management Engine BIOS Extension) es la interfaz de configuración de este subsistema, accesible como un menú dentro del BIOS/UEFI. Desde ahí se puede configurar cosas como la administración remota del equipo (Intel AMT).

- Intel CSME (Converged Security and Management Engine): es el nombre nuevo que Intel le dio al MEBx a partir de la versión 11 de AMT. Es la misma idea pero con una arquitectura actualizada.

Esto permite a los administradores de IT gestionar equipos de forma remota: encender/apagar la máquina, acceder al escritorio via KVM, redigir el boot desde una imagen remota, todo sin importar el estado del sistema operativo.

#### 4. ¿Qué es coreboot ? ¿Qué productos lo incorporan ?¿Cuales son las ventajas de su utilización?

Coreboot es un proyecto de firmware open-source diseñado para reemplazar el BIOS/UEFI propietario. Está enfocado en velocidad de arranque, seguridad y flexibilidad. Hace la mínima cantidad de inicialización de hardware necesaria y después le pasa el control al sistema operativo.

Los productos que lo incorporan son:
- Google Chromebooks: uno de los mayores usuarios de coreboot.
- System76: fabricante de PCs Linux que usa coreboot con firmware open-source.
- Purism: notebooks enfocadas en privacidad, con coreboot y el Intel ME neutralizado.
- StarLabs: notebooks Linux con opción de deshabilitar el Intel ME desde coreboot.
- Protectil: hardware de networking con firmware coreboot.
- Tesla Model 3: usa coreboot como bootloader en la computadora del auto.
- NovaCustom y Nitrokey: notebooks con la distribución Dasharo basada en coreboot.

Las ventajas que presenta coreboot son:
- Velocidad: Tiempos de arranque menores 2.5 segundos en la mayoría de placas.
- Seguridad: Al ser open-source, la comunidad puede auditar el código y encontrar vulnerabilidades, a diferencia del BIOS propietario donde el código es cerrado. Además soporta funciones como verified boot.
- Sin costos de licencia: Al ser open-source, no tiene fees de licencia y recibe contribuciones de cientos de desarrolladores al año.
- Transparencia: no tiene backdoors ocultos como los que se han encontrado en firmware propietario.
- Flexibilidad: Soporta múltiples arquitecturas (x86, ARM, ARM64, MIPS, RISC-V) y puede cargar distintos payloads como SeaBIOS, Linux, o UEFI.

## Modo protegido

#### ¿Cómo sería un programa que tenga dos descriptores de memoria diferentes, uno para cada segmento (código y datos) en espacios de memoria diferenciados? 

Actualmente nuestro programa tiene dos descriptores (código y datos) pero ambos con base y límite de 4 GB, es decir, comparten todo el espacio de memoria. Para tener espacios diferenciados habría que modificar la base de cada descriptor.

Por ejemplo, si se quisiera cambiar el segmento de código ocupe los primeros 64 KB empezando en `0x00010000`, habría que cambiar la GDT de esta manera:

En `gdt_code`, la base queda en 0 y el límite se reduce:

```asm
gdt_code:
    .word 0xFFFF       # Límite 0-15 (64KB)
    .word 0x0000       # Base 0-15 = 0x0000
    .byte 0x00         # Base 16-23 = 0x00
    .byte 0b10011010   # Acceso (igual)
    .byte 0b01000000   # Granularidad byte, límite 16-19 = 0
    .byte 0x00         # Base 24-31 = 0x00
```

en `gdt_data`, la base cambia `0x00010000`

```asm
gdt_data:
    .word 0xFFFF       # Límite 0-15 (64KB)
    .word 0x0000       # Base 0-15 = 0x0000
    .byte 0x01         # Base 16-23 = 0x01 (acá cambia)
    .byte 0b10010010   # Acceso (igual)
    .byte 0b01000000   # Granularidad byte, límite 16-19 = 0
    .byte 0x00         # Base 24-31 = 0x00
```

La ventaja que presenta esto es que cada segmento tiene su propia zona de memoria y no se pueden pisar entre sí. Si el código intenta acceder fuera de su límite o los datos intentan acceder fuera de su límite o los datos intentan ejecutarse, el procesador genera una excepción (General Protection Fault).


#### Cambiar los bits de acceso del segmento de datos para que sea de solo lectura, intentar escribir, ¿Que sucede? ¿Que debería suceder a continuación? (revisar el teórico) Verificarlo con gdb.

Para demostrar qué sucede al cambiar el segmento de datos a solo lectura, se modificó el byte de de acceso del descriptor `gdt_data` en la GDT.

El byte de acceso original es `0b10010010`, donde el bit 1 (escritura) está en 1, lo que permite leer y escribir en el segmento de datos. Se cambió a `0b10010000`, poniendo el bit 1 en 0, haciendo el segmento de solo lectura.

```asm
# Original - lectura/escritura
.byte 0b10010010

# Modificado - solo lectura
.byte 0b10010000
```

Con el segmento de datos con escritura habilitada, el programa escribe "Hello" en la memoria VGA (`0xB80000`) y se muestra correctamente en pantalla:

![](/tp3/img/1.png)

Al cambiar a solo lectura, cuando el código intenta escribir en `0xB8000`, el procesador detecta que el descriptor de datos no tiene permisos de escritura y genera una General Protection Fault. Como no hay una IDT (Interrupt Descriptor Table) configurada para manejar esa excepción, se produce un triple fault: la excepción genera otra excepción, que genera otra, y el procesador se cuelga. En qemu se ve que el sistema no muestra nada después de  "Booting from Hard Disk...".

![](/tp3/img/2.png)

Esto demuestra el mecanismo de protección de memoria del modo protegido: los bits de acceso de la GDT permiten controlar los permisos de cada segmento a nivel de hardware, y cualquier violación es detectada automáticamente por el procesador.

#### En modo protegido, ¿Con qué valor se cargan los registros de segmento? ¿Porque?

En modo protegido, los registros de segmento se cargan con selectores que son offsets dentro de la GDT.

El registro CS (code segment) se carga con `0x08`, que apunta al descriptor de código (`gdt_code`), ubicado en la segunda entrada de la GDT (offset 1x8 = 8). Esta carga se realiza mediante un salto largo (`ljmp $0x08, $protected_mode`) ya que es la única forma de modificar CS.

Los registros DS, ES, FS, GS y SS se cargan con `0x10`, que apunta al descriptor de datos (`gdt_datos`), ubicado en la tercera entrada de la GDT (offset 2x8 = 16 = 0x10).

```asm
mov $0x10, %ax
mov %ax, %ds
mov %ax, %es
mov %ax, %fs
mov %ax, %gs
mov %ax, %ss
```

Se tienen estos valores porque en modo protegido los registros de segmento ya no contienen direcciones directas como en modo real. Contienen selectores que el procesador usa como índice para buscar en la GDT la base, el límite y los permisos del segmento correspondiente. El valor `0x00` no se puede usar porque la primera entrada de la GDT es obligatoriamente nula, así que los selectores válidos empiezan desde `0x08`.