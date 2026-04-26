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

### UEFI y coreboot

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

## Linker

#### 1. ¿Qué es un Linker? ¿Que hace?

Un linker es un programa del sistema que actúa como el paso final en el proceso de compilación de software. Su propósito general es tomar uno o más archivos objeto (los archivos `.o` o `.obj` generados por el compilador o el ensamblador) y combinarlos para producir un único archivo ejecutable, una biblioteca o un archivo objeto más grande.

Las tareas principales que realiza un linker se dividen en dos grandes categorías:

1. **Resolución de símbolos:** En proyectos con múltiples archivos fuente, es común que un archivo haga referencia a una función o variable definida en otro distinto. El compilador, al procesar los archivos de a uno por vez, deja estas referencias "abiertas" o sin resolver. El linker se encarga de recorrer todos los archivos objeto, buscar dónde está realmente definida cada función o variable, y enlazar esa referencia con su definición exacta.
2. **Reubicación y Organización de Memoria:** El compilador procesa cada archivo asumiendo por defecto que el código comenzará en la dirección de memoria `0`. El linker toma las diferentes secciones de los archivos objeto (como la sección `.text` para código ejecutable, `.data` para variables inicializadas, y `.bss` para variables sin inicializar) y las unifica. Luego, calcula y asigna las direcciones de memoria definitivas donde residirá cada parte del programa al momento de ejecutarse, ajustando todos los punteros y saltos para que funcionen correctamente.

#### 2. ¿Que es la dirección que aparece en el script del linker?¿Porqué es necesaria ?

Esa direccion es utilizada para manejar la dirección base de memoria (VMA o _Virtual Memory Address_) en la cual el programa será cargado físicamente en la RAM para su ejecución. En el _Linker Script_ se define típicamente con el símbolo `.` (por ejemplo, `. = 0x7c00;`). Esto es fundamental en el desarrollo a bajo nivel, como en la escritura de un bootloader o un kernel para pasar a **modo protegido**, ya que el hardware impone reglas estrictas. Por ejemplo, en la arquitectura x86, el BIOS siempre carga el sector de arranque (MBR) exactamente en la dirección de memoria física `0x7C00`.

Esta dirección es **estrictamente necesaria** porque le informa al linker cuál será la ubicación física real del programa en la memoria RAM. Si no se usara el script, el linker calcularía todas las direcciones absolutas asumiendo que el programa arranca en `0x0`. Como resultado, cuando el código intente leer una variable o saltar a una función una vez cargado por el BIOS, buscará en la dirección equivocada de la RAM y el sistema fallará. El linker script garantiza que las referencias de memoria compiladas coincidan con la realidad física del hardware.

#### 3. Comparacion de la salida de objdump con hd

#### 4. Grabacion de la imagen en un pendrive

#### 5. ¿Para qué se utiliza la opción `--oformat binary` en el linker?

Esta opcion le indica al linker que el archivo ejecutable resultante debe ser un **binario plano (flat binary)**. Esto significa que el archivo final será crudo y contendrá exclusivamente el código máquina y los datos, eliminando por completo cualquier tipo de metadatos, cabeceras o tablas de símbolos que los linkers suelen agregar por defecto (como los formatos estándar ELF en Linux).

En el desarrollo a bajo nivel (como al hacer un bootloader para pasar a modo protegido), el código es cargado directamente por el BIOS en la memoria. A diferencia del sistema operativo, el BIOS no sabe leer formatos de archivos complejos ni entiende de cabeceras; simplemente lee sectores del disco, los pone en memoria y empieza a ejecutar lo que haya ahí byte por byte. Si el archivo tuviera cabeceras ELF, el procesador intentaría "ejecutar" esos metadatos como si fueran instrucciones, lo que haría que el sistema crashee instantáneamente. La opción `--oformat binary` asegura que al procesador solo le lleguen las instrucciones puras que debe ejecutar.
