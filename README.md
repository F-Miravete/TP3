# TESTING de SOFTWARE (CESE 20CO)
# Trabajo Práctico Número 3 (Flavio Miravete) TEST UNITARIOS

## Uso del repositorio

Este repositorio utiliza las siguientes herramientas:

1. [clang-format] () para el mantenimiento del codigo escrito en lenguaje C
2. [pre-commit] (https://pre-commit.com/#install) para validaciones generales de formato de repositorio
3. [Ceedling] () para ejecutar las pruebas unitarias en forma automatizada
4. [lcov] () para generar los informes de cobertura de las pruebas unitarias

Después de clonar el repositorio usted debería ejecutar el siguiente comando:

```
pre-commit install
```

Para ejecutar las pruebas unitarias se utiliza el siguiente comando:

```
ceedling test:all
```

Para generar informes de cobertura de las pruebas se utiliza el siguiente comando:

```
ceedling clobber gcov:all utils:gcov
```

Para generar la documentacion del proyecto se utiliza el siguiente comando:

```
make doc
```
