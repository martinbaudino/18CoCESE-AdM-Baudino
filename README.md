# 18CoCESE-AdM-Baudino - repositorio público
## **Arquitectura de Microprocesadores** - Carrera de Especialización en Sistemas Embebidos - Universidad de Buenos Aires - BAUDINO, Martín - 2022

### Preguntas orientadoras
1. Describa brevemente los diferentes perfiles de familias de microprocesadores/microcontroladores de ARM. Explique alguna de sus diferencias características.

La empresa ARM divide su oferta de procesadores (CPUs) Cortex en en tres perfiles: 
- **Cortex-A** para **Aplicaciones** que utilizan Sistemas Operativos de Propósitos Generales (GPOSs), para los que proveen una Unidad de Manejo de Memoria (MMU).
- **Cortex-R** para aplicaciones de seguridad crítica y tiempo **Real**, con características distintivas como Memoria Estrechamente Acoplada (TCM) y menejo determinístico de interrupciones.
- **Cortex-M** son núcleos pensados para ser utilizados como **Microcontroladores** de 32-bits que pueden ser programados en C directamente generando alta densidad de código.

### Cortex M

1. Describa brevemente las diferencias entre las familias de procesadores Cortex M0, M3 y M4.

Los procesadores Cortex M0 y M0+ tienen un set reducido de instrucciones para optimizar el consumo energético y el costo de fabricación. Las familias M3 y M4 implementan los sets de instrucciones Thumb completos y ofrecen mayor performance y funcionalidades.

2. ¿Por qué se dice que el set de instrucciones Thumb permite mayor densidad de código? Explique

Para evaluar la densidad del código de una arquitectura se incluye tanto el tamaño en bits de cada instrucción, como la cantidad de instrucciones que se deben utilizar para realizar tareas típicas. El set de instrucciones Thumb codifica algunas instrucciones en 16 bits en vez de 32 bits, manteniendo la funcionalidad, por lo que se reduce la cantidad de memoria de programa que ocupa la aplicación. 

3. ¿Qué entiende por arquitectura load-store? ¿Qué tipo de instrucciones no posee este tipo de arquitectura?

En la arquitectura load-store las instrucciones se dividen en dos tipos: las que acceden a memoria para cargar (load) o guardar (store) valores en (o desde) los registros, y las que utilizan la ALU para realizar operaciones sobre los valores que se encuentran en registros. En este tipo de arquitectura no se pueden realizar operaciones entre direcciones de memoria y registros de manera directa, es necesario primero cargar los valores en los registros.

4. ¿Cómo es el mapa de memoria de la familia?

La familia utiliza direcciones de 32 bits y un espacio de direccionamiento de memoria unificado, por lo que presenta un mapa de memoria plano de 4GB donde se encuentran la memoria de programa, la de aplicación y los periféricos.

5. ¿Qué ventajas presenta el uso de los “shadowed pointers” del PSP y el MSP?

Permite configurar a un Sistema Operativo para que utilice el Puntero a la Pila Principal (MSP) y las aplicaciones de usuario (tareas) para que utilicen el Puntero a la Pila de Procesos (PSP). De esa manera el OS tiene garantizada la gestión de la Pila Principal y el cambio de la pila en uso se realiza automáticamente en la gestión de excepciones.

6. Describa los diferentes modos de privilegio y operación del Cortex M, sus relaciones y como se conmuta de uno al otro. Describa un ejemplo en el que se pasa del modo privilegiado a no priviligiado y nuevamente a privilegiado.

Los modos de operación se dividen en **Handler** (dentro de manejadores de excepción) y **Thread** (ejecución normal del código de aplicación). El modo **Thread** puede tener nivel de acceso privilegiado o no privilegiado, mientras que el modo **Handler** siempre tiene nivel de acceso privilegiado. La separación en niveles de acceso privilegiado y no privilegiado define dos tipos de permisos de acceso a áreas de memoria y también el acceso a instrucciones especiales. Como ejemplo, un RTOS se ejecuta en modo **Thread** con acceso privilegiado y la mayoría de las tareas en modo **Thread** con acceso no privilegiado. La interrupción del SysTick se utiliza para suspender una tarea de ejecución y pasar al modo **Handler** y luego de esa manera devolverle la ejecución al RTOS.

7. ¿Qué se entiende por modelo de registros ortogonal? Dé un ejemplo
Cuando se habla de ortogonal en este contexto se refiere a que el tipo de instrucción y los modos de direccionamiento varían de manera indipendiente, análogo a variables independientes en un sistema de coordenadas ortogonales. El resultado de esto es que todos los tipos de instrucciones pueden utilizar todos los tipos de direccionamiento de memoria.


8. ¿Qué ventajas presenta el uso de intrucciones de ejecución condicional (IT)? Dé un ejemplo



9. Describa brevemente las excepciones más prioritarias (reset, NMI, Hardfault).



10. Describa las funciones principales de la pila. ¿Cómo resuelve la arquitectura el llamado a funciones y su retorno?



11. Describa la secuencia de reset del microprocesador.



12. ¿Qué entiende por “core peripherals”? ¿Qué diferencia existe entre estos y el resto de los periféricos?



13. ¿Cómo se implementan las prioridades de las interrupciones? Dé un ejemplo



14. ¿Qué es el CMSIS? ¿Qué función cumple? ¿Quién lo provee? ¿Qué ventajas aporta?



15. Cuando ocurre una interrupción, asumiendo que está habilitada ¿Cómo opera el microprocesador para atender a la subrutina correspondiente? Explique con un ejemplo



17. ¿Cómo cambia la operación de stacking al utilizar la unidad de punto flotante?



18. Explique las características avanzadas de atención a interrupciones: tail chaining y late arrival.



19. ¿Qué es el systick? ¿Por qué puede afirmarse que su implementación favorece la portabilidad de los sistemas operativos embebidos?



20. ¿Qué funciones cumple la unidad de protección de memoria (MPU)?



21. ¿Cuántas regiones pueden configurarse como máximo? ¿Qué ocurre en caso de haber solapamientos de las regiones? ¿Qué ocurre con las zonas de memoria no cubiertas por las regiones definidas?



22. ¿Para qué se suele utilizar la excepción PendSV? ¿Cómo se relaciona su uso con el resto de las excepciones? Dé un ejemplo



23. ¿Para qué se suele utilizar la excepción SVC? Expliquelo dentro de un marco de un sistema operativo embebido.

### ISA

1. ¿Qué son los sufijos y para qué se los utiliza? Dé un ejemplo



2. ¿Para qué se utiliza el sufijo ‘s’? Dé un ejemplo



3. ¿Qué utilidad tiene la implementación de instrucciones de aritmética saturada? Dé un ejemplo con operaciones con datos de 8 bits.



4. Describa brevemente la interfaz entre assembler y C ¿Cómo se reciben los argumentos de las funciones? ¿Cómo se devuelve el resultado? ¿Qué registros deben guardarse en la pila antes de ser modificados?



5. ¿Qué es una instrucción SIMD? ¿En qué se aplican y que ventajas reporta su uso? Dé un ejemplo.
