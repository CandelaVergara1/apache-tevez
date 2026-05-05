# Trabajo Práctico 1: El rendimiento de las computadoras

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

## Introducción

En este trabajo práctico se tiene como objetivo el rendimiento de sistemas de cómputo. Por un lado, el análisis de hardware mediante benchmarks de tercero, y por otra parte, la medición de performance de código propio mediante herramientas de profiling.

## 1. Armar una lista de benchmarks por tarea diaria

Un benchmark es un programa de prueba que mide el rendimiento de un sistema informático. La elección del benchmark más adecuado depende de las tareas que el usuario realiza habitualmente, ya que el rendimiento dependerá la aplicación que se va a hacer.
Los benchmarks se clasifican en 4 tipos:

- Sintéticos: programas artificiales diseñados para estresar un componente específico del sistema.
- Reducidos: fragmentos pequeños de código extraídos de programas reales.
- Kernel: aíslan la parte computacionalmente más intensiva de una aplicación real.
- Programas reales: utilizan aplicaciones completas que el usuario usa cotidianamente.

| Tareas diaria                      | Benchmark        | Tipo      |
| ---------------------------------- | ---------------- | --------- |
| Rendimiento de funciones de python | pytest-benchmark | Kernel    |
| Comprimir archivos                 | 7-zip            | Sintetico |
| Rendimiento de GPU                 | 3DMark           | Sintetico |
| Operaciones de punto flotante      | LINKPACK         | Kernel    |

---

## 2. Rendimiento en compilación del kernel Linux

A continuación se vera el rendimiento en compilación del kernel de Linux para 3 diferentes procesadores:

- Plataforma de benchmarking: Phoronix Test Suite

| Procesadores              | Tiempo Promedio (seg) |
| ------------------------- | --------------------- |
| Intel core i5-13600K      | $72^{+/-5}$           |
| AMD Ryzen 9 5900X 12-Core | $76^{+/-8}$           |
| AMD Ryzen 9 7950X 16-Core | $50^{+/-6}$           |

A partir de los tiempos de compilación obtenidos, se puede calcular el rendimiento, el speedup y la eficiencia en compilación del kernel de Linux.

### Rendimiento

El rendimiento del procesador se define como el inverso del tiempo de ejecución, es decir, a menor tiempo de compilación, mayor rendimiento. A continuación se calcula el rendimiento de cada procesador en base a los tiempos obtenidos

$$
\eta_{prog} = \frac{1}{T_{prog}}
$$

| Procesadores              | Rendimiento |
| ------------------------- | ----------- |
| Intel core i5-13600K      | 0.0139      |
| AMD Ryzen 9 5900X 12-Core | 0.0132      |
| AMD Ryzen 9 7950X 16-Core | 0.02        |

> Podemos observar que el procesador AMD Ryzen 9 7950X demuestra el mayor rendimiento en compilación del kernel de Linux con un tiempo de 50 segundos

### Speedup

El speedup indica cuántas veces más rápido es un procesador respecto a uno de referencia (usaremos de referencia primero el procesador Intel core i5-13600K y posteriormente el AMD Ryzen 9 5900X 12-Core). Se calcula dividiendo el tiempo del procesador base por el tiempo del procesador a evaluar.

$$
Speedup = \frac{\text{Rendimiento mejorado}}{\text{Rendimiento original}} = \frac{EX_\text{CPU Original}}{EX_\text{CPU Mejorado}}
$$

- Tomando el procesador Intel core i5-13600K como referencia:

| Procesadores              | Speedup |
| ------------------------- | ------- |
| Intel core i5-13600K      | 1.000   |
| AMD Ryzen 9 5900X 12-Core | 0.947   |
| AMD Ryzen 9 7950X 16-Core | 1.440   |

> Tomando como referencia el i5-13600K el Ryzen 9 7950X logra un speedup de 1.44, es decir, compila un 44% más rápido. El Ryzen 9 5900X en cambio obtiene un speedup de 0.947, siendo más lento que la referencia

- Tomando el procesador AMD Ryzen 9 5900X 12-Core como referencia:

| Procesadores              | Speedup |
| ------------------------- | ------- |
| Intel core i5-13600K      | 1.056   |
| AMD Ryzen 9 5900X 12-Core | 1.000   |
| AMD Ryzen 9 7950X 16-Core | 1.520   |

> Al tomar el 5900X como referencia el 7950X sigue siendo el más rápido con un speedup de 1.52, y el i5-13600K lo supera con un speedup de 1.056. En ambos casos el AMD Ryzen 9 7950X demuestra la mayor aceleración, consolidándose como el procesador más rápido para esta tarea.

### Eficiencia

La eficiencia mide qué tan bien aprovecha un procesador sus recursos disponibles en relación al speedup obtenido. En este caso se analiza la eficiencia en función de la cantidad de núcleos, dividiendo el speedup de cada procesador por su número de núcleos.

$$
Eficiencia = \frac{Speedup}{n}
$$

- Intel core i5-13600K como referencia:

| Procesadores              | Cores | Eficiencia |
| ------------------------- | ----- | ---------- |
| Intel core i5-13600K      | 14    | 0.071      |
| AMD Ryzen 9 5900X 12-Core | 12    | 0.079      |
| AMD Ryzen 9 7950X 16-Core | 16    | 0.090      |

- AMD Ryzen 9 5900X 12-Core como referencia:

| Procesadores              | Cores | Eficiencia |
| ------------------------- | ----- | ---------- |
| Intel core i5-13600K      | 14    | 0.075      |
| AMD Ryzen 9 5900X 12-Core | 12    | 0.083      |
| AMD Ryzen 9 7950X 16-Core | 16    | 0.095      |

> En ambas referencias el AMD Ryzen 7950x obtiene la mayor eficiencia por núcleo, aprovechando mejor cada uno de sus 16 núcleos disponibles. Le sigue el Ryzen 9 5900x, que a pesar de tener menor rendimiento absoluto que el i5-13600k, logra una mayor eficiencia por núcleo al tener menos núcleos en total

A continuación se analizara también la eficiencia en términos económicos y energéticos. Para esto se analiza la eficiencia en costo tomando un precio de referencia de cada procesador y su consumo energético (TDP), permitiendo determinar cuál ofrece la mejor relación entre rendimiento obtenido y recursos invertidos.

$$
Eficiencia = \frac{Speedup}{Precio}
$$

$$
Eficiencia = \frac{Speedup}{TDP}
$$

- Intel core i5-13600K como referencia:

| Procesadores              | Costo (USD) | Eficiencia |
| ------------------------- | ----------- | ---------- |
| Intel core i5-13600K      | 320         | 0.00312    |
| AMD Ryzen 9 5900X 12-Core | 270         | 0.00351    |
| AMD Ryzen 9 7950X 16-Core | 498         | 0.00289    |

| Procesadores              | TDP (W) | Eficiencia |
| ------------------------- | ------- | ---------- |
| Intel core i5-13600K      | 125     | 0.0080     |
| AMD Ryzen 9 5900X 12-Core | 105     | 0.0091     |
| AMD Ryzen 9 7950X 16-Core | 170     | 0.0085     |

- Intel core AMD Ryzen 9 5900X 12-Core como referencia:

| Procesadores              | Costo (USD) | Eficiencia |
| ------------------------- | ----------- | ---------- |
| Intel core i5-13600K      | 320         | 0.0033     |
| AMD Ryzen 9 5900X 12-Core | 270         | 0.0037     |
| AMD Ryzen 9 7950X 16-Core | 498         | 0.0031     |

| Procesadores              | TDP (W) | Eficiencia |
| ------------------------- | ------- | ---------- |
| Intel core i5-13600K      | 125     | 0.0085     |
| AMD Ryzen 9 5900X 12-Core | 105     | 0.0095     |
| AMD Ryzen 9 7950X 16-Core | 170     | 0.0089     |

> En ambas referencias el AMD Ryzen 9 5900X resulta ser el más eficiente en términos de costo ofreciendo la mejor relación entre speedup y precio. El AMD Ryzen 9 7950X, a pesar de ser el más rápido, resulta el menos eficiente económicamente debido a su alto precio. El i5-13600K se ubica en un punto intermedio.
> En términos energéticos el AMD Ryzen 9 5900X también resulta el más eficiente en ambas referencias, logrando la mejor relación entre speedup y consumo eléctrico gracias a su bajo TDP de 105W. El AMD Ryzen 9 7950X, si bien obtiene el mayor rendimiento absoluto, consume significativamente más energía. El i5-13600K se posiciona entre ambos.

## Parte 3: Práctico ESP32

Para esta parte, se utiliza un microcontrolador ESP32, el cual se le puede variar la frecuencia. Se tiene que ejecutar un código que dure alrededor de 10 segundos, el cual debe ejecutar operaciones con números enteros y flotantes. Se tiene que variar la frecuencia del microcontrolador y verificar los tiempos.

Para esta prueba nos valemos del siguiente código:

```cpp
#include "esp32-hal-cpu.h"

void setup() {
Serial.begin(115200);
delay(1000);

// 1. Probamos a 80 MHz (Velocidad baja)
setCpuFrequencyMhz(80);
delay(500);
ejecutarPrueba();

// 2. Probamos a 160 MHz (Velocidad estándar)
setCpuFrequencyMhz(160);
delay(500);
ejecutarPrueba();

}

void ejecutarPrueba() {
uint32_t freq = getCpuFrequencyMhz();
Serial.printf("\n--- Test a %d MHz ---\n", freq);
Serial.flush();

long iteraciones = 100000000; // Mantenemos el mismo trabajo
volatile int suma = 0;

long t0 = micros();
for (long i = 0; i < iteraciones; i++) {
suma += 1;
}
long t1 = micros();

Serial.printf("Tiempo: %.4f segundos\n", (t1 - t0) / 1000000.0);
Serial.flush();
}

void loop() {}
```

### Salida del programa, variando la frecuencia de clock de la ESP32

![Output programa](profiling/imgs/esp32.jpg)

Como se puede apreciar, al tener una frecuencia del clock de 80Mhz, el programa demora 13,9 segundos. Por otro lado, cuando la frecuencia aumenta al doble, a 160Mhz, el programa demora solo 6,91 segundos, tardando prácticamente la mitad de tiempo.

---

Así, se calcula el rendimiento para cada ejecución del programa:

### 1. Configuración a 80 MHz

Con un tiempo de ejecución registrado de **13,9056 s**:

$$\eta_{80MHz} = \frac{1}{13,9056} \approx 0,07191 $$

### 2. Configuración a 160 MHz

Con un tiempo de ejecución registrado de **6,9137 s**:

$$\eta_{160MHz} = \frac{1}{6,9137} \approx 0,14464 $$

Al duplicar la frecuencia de 80 MHz a 160 MHz, el rendimiento se incrementó en un factor de **2,011**, lo que indica una eficiencia casi lineal respecto al aumento de reloj.

### Cálculo del Speedup

Para verificar el incremento en el rendimiento, se calcula el Speedup, comparando el rendimiento original, que es de **80 Mhz**, con el rendimiento mejorado, que es de **160 Mhz**

$$Speedup = \frac{13,9056 \, s}{6,9137 \, s}$$

$$Speedup \approx \mathbf{2,0113}$$

El programa ejecutado a **160 MHz** es aproximadamente **2,01 veces más rápido** que a **80 MHz**. Esto demuestra que la mejora en la frecuencia de reloj se traslada de forma casi directamente proporcional al rendimiento total en este caso.

---

### Justificación: Efecto de la variación de frecuencia en el tiempo de ejecución

El tiempo de ejecución de un programa ($T_{prog}$) es inversamente proporcional a la frecuencia de reloj del procesador ($F_{clk}$). Esta relación se rige por la ecuación fundamental del tiempo de CPU:

$$T_{prog} = \text{Instrucciones} \times CPI \times \frac{1}{F_{clk}}$$

Al duplicar la frecuencia de reloj (de 80 MHz a 160 MHz), el período de cada ciclo de reloj se reduce a la mitad. Asumiendo que la cantidad de instrucciones ejecutadas y el promedio de ciclos por instrucción (CPI) se mantienen constantes para el mismo programa, el sistema procesa la misma carga de trabajo en la mitad del tiempo.

Los resultados empíricos corroboran este comportamiento teórico:

- **Tiempo a 80 MHz:** 13,9056 s
- **Tiempo a 160 MHz:** 6,9137 s

La reducción del tiempo es de aproximadamente el 50% (un valor ideal sería 6,9528 s). El hecho de obtener 6,9137 s demuestra un escalado de rendimiento casi perfecto, resultando en un Speedup de aproximadamente 2,01. La diferencia marginal entre el valor puramente teórico y el registrado se debe a que ciertas operaciones del sistema, como la latencia de memoria o los buses de entrada/salida, pueden tener tiempos de respuesta que no escalan de forma idéntica a la frecuencia del núcleo del procesador.

En conclusión, la variación en la frecuencia de reloj impacta de manera directa y proporcional en la velocidad del sistema: duplicar la frecuencia implica reducir el tiempo de ejecución a la mitad.

## Parte 4: Profiling (gprof)

El profiling es el proceso de medir y analizar el rendimiento de un código, evaluando principalmente el tiempo de ejecución del programa, como asi también cuanto tiempo tarda en ejecutarse cada función. Permite identificar qué partes del código consumen más recursos, mediante herramientas llamadas profilers, que suelen utilizar técnicas como muestreo (perf) o inyección de código (gprof).

A partir de la realización del tutorial descrito en time profiling se pudo realizar el gprof de test_gprof.c y test_gprof_new.c, del cual obtuvimos un archivo txt que nos dio los resultados para el análisis ya que contiene toda la información de perfil deseada. Como ejemplo subimos tres archivo .txt con los resultados de los tres integrantes del grupo. Se muestra a continuación los resultados de analisis_Salinas-Joaquin.txt, el cual contiene dos tablas importantes:

- **Perfil Plano:** Brinda una descripción general de la información de tiempo de las funciones, como el consumo de tiempo para la ejecución de una función en particular, cuántas veces se llamó, etc.

  ![Captura del perfil plano](profiling/imgs/FlatProfile.png)

  Siendo:
  - Self seconds: tiempo de ejecución propio de cada función.
  - %Time: porcentaje del tiempo total consumido por la función.
  - Total seconds: es el tiempo de la función + el de las funciones que llama (sus hijos).
  - Calls: cantidad de veces que fue llamada.

- **Gráfico de llamadas:** representa las relaciones entre funciones, mostrando qué funciones llaman a una determinada función y cuáles son invocadas desde ella. Esto permite analizar la estructura de ejecución del programa y estimar el tiempo empleado en cada subrutina.

  ![Captura del grafico de llamadas](profiling/imgs/Callgraph.png)

A continuación se muestra el gráfico de llamadas generado por gprof2dot, 
el cual representa visualmente las relaciones entre funciones y el tiempo 
consumido por cada una:

![Gráfico de llamadas](profiling/imgs/gprof2dot.png)
## Conclusiones sobre el uso del tiempo de las funciones

A partir del análisis de los resultados del profiling se observó que la función func1 es la que mayor tiempo consume, representando aproximadamente el 39.21% del tiempo total de ejecución, esto indica que es la principal candidata a optimización, ya que es donde más tiempo pasa el programa. En segundo lugar, la función new_func1 utiliza alrededor del 35.53% del tiempo, esto sugiere que también tiene un impacto significativo en el rendimiento general.

Por otro lado, la función func2 consume un 25% del tiempo total, lo que la ubica como una función de importancia media en términos de consumo de recursos.
Finalmente, la función main tiene un impacto prácticamente despreciable (0.26% del tiempo total), lo cual es esperable, ya que generalmente se encarga solo de la coordinación del flujo del programa.
En conclusión, el profiling permite identificar que la mayor parte del tiempo de ejecución se concentra en pocas funciones (principalmente func1 y new_func1), lo cual es clave para enfocar esfuerzos de optimización de manera eficiente.
