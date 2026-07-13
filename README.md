# Turing-Machine-Checker

- Obejtivo
    - Verificar que el comportamiento de una maquina de Turing dado el diseño de la misma y una entrada, es el esperado
    - "El esperado", depende de la persona que diseño la maquina, puede verificarse mediante la descripción instantánea al momento de llegar a un estado final, como solicitar detener la maquina en algún momento especifico
- Comportamiento
    - Se carga la Maquina de Turing en un archivo de nombre “machine.in”
    - Se carga el dato de entrada en un archivo de nombre “input.in”
    - El programa comprueba primero que el formato de ambos archivos es correcto
    - El programa luego de comprobar el formato, el programa consulta si se quiere
    detener la maquina en un estado específico, de responder que si, se muestran los
    estados disponibles para seleccionar dicho estado, de responder que no, la
    maquina se detendrá al llegar a un estado final.
    - El programa luego de determinar cuando se detendrá la maquina, comenzara a
    mostrar las descripciones instantáneas de cada transición dado el Input y según las
    reglas de transición indicadas en Machine
    - El programa se deberá detener si se llega a un estado final o un estado
     previamente indicado por el usuario, dejando por ultimo la descripción instantánea a la que llego.
- Limitaciones: El problema de la parada (Halting Problem)
    - Este programa **no puede** determinar si la máquina de Turing llegará
      a un estado final o entrará en un bucle infinito para una entrada dada.
      El Halting Problem (Turing, 1936) demuestra que es imposible diseñar
      un algoritmo general que decida si una máquina de Turing se detiene.
    - Las validaciones que realiza el programa son exclusivamente de formato
      y consistencia estructural:
        - **Machine:**
            - Q: estados repetidos
            - Estado inicial $q_0 \in Q$
            - F: estados repetidos, y $F \subseteq Q$
            - $\Sigma$: símbolos repetidos
            - $\Gamma$: símbolos repetidos, y $\Sigma \subseteq \Gamma$
            - Blank symbol $\in \Gamma - \Sigma$
            - Cada regla $\delta$: estados usados $\in Q$, símbolos usados $\in \Gamma$
        - **Input:**
            - Cada símbolo de la cadena $\in \Sigma$
    - El usuario debe ser consciente de que, aunque el formato sea correcto,
      la máquina podría no detenerse nunca. No hay manera de comprobarlo
      previamente a la ejecución.
- Bibliografias, notaciones
    - Según la guia 5 de la materia Lenguages Formales y Computabilidad de la carrera Licenciatura en Ciencias de la Computación de UNC-FAMAF
- Formatos
    - "machine.in"
        - Se espera que:
            1. La 1ra linea sean los estados $Q$
            2. La 2da linea sea el estado inicial $q_0$ y que este esté en el conjunto de estados $q_0\in Q$
            3. La 3ra linea sean los estados finales $F$ y que estos esten entre los estados $F\subseteq Q$
            4. La 4ta linea sea el alfabeta de entrada $\Sigma$
            5. La 5ta linea sea el alfabeto $\Gamma$ y que $\Gamma$ contenga a $\Sigma$
            6. La 6ta linea sea el _Blank symbol_ y que _Blank symbol_ $\in\Gamma-\Sigma$
            7. Desde la 7ma en adelante sean las reglas de $\delta$
        - Ejemplo:
            ```
            {q0,q1,q2,q3,q4,q5}
            q0
            {q4,q5}
            {@,%,$}
            {@,%,$,B}
            B
            (q0, B) = (q1,B,R)
            (q1, B) = (q2,@,K)
            (q2, @) = (q3,%,L)
            (q3, %) = (q4,$,R)
            ...
            ```
    - "input.in"
        - Se espera que:
            - La 1ra linea tenga una cadena $\alpha\in\Sigma^*$
        - Ejemplo:
            ```
            @@$@%@%%@
            ```
- Modo de uso:
    - Se cargan los archivos ```machine.in``` y ```input.in``` de manera que cumplan con el formato como en el ejemplo.
    - Y luego se corre lo siguiente en la terminal con ambos archivos en el mismo directorio que el programa
    ```bash
    $ make run
    ```
