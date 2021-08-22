&nbsp;
&nbsp;

<img src="misc/logo.png" width="600" alt="Corne QMK hysp keymap Alvaro Prieto Lauroba"/>

# Custom QMK keymap for [Corne Keyboard](https://github.com/foostan/crkbd) 
Developed by Álvaro Prieto Lauroba

[🇬🇧 Description](#-description)    [🇪🇸 Descripción](#-descripción)    [⬇️ Descarga / download](#%EF%B8%8F-descarga)

<img src="extra/keymap.png" width="100%" alt="QMK Corne Keyboard layout RSI"/>


# 🇬🇧 Description

A repetitive strain injury (RSI) is an injury to part of the musculoskeletal or nervous system caused by repetitive use, vibrations, compression or long periods in a fixed position. This keyboard layout has been designed to minimize postural problems associated with the placement of certain keys and symbols commonly used by software developers. To achieve this, it has been attempted to reduce the useful area of the keyboard to the minimum, placing symbols in positions close to the strongest fingers (such as thumb and index fingers).

The following concepts have been used to put it into practice:

- Usage of a [split keyboard](https://www.r-go-tools.com/products/ergonomic-keyboards/r-go-split/) to improve the separation between both hands, placing them parallel to the shoulders. At the same time this makes it possible to use the space bar as two different keys.

- Extensive use of modifier keys (L1, L2, L3, L4 and a contextual modifier: the App layer). These modifiers allow you to stack different symbols on the same key.

- Usage of double keystrokes to write complementary or alternative symbols (marked with a triangle ▲) 

- Usage of a contextual modifier: App Layer, which allows some keys to have specific behaviors in each program, something very useful for personalized per-app keyboard shortcuts.

- Usage of textured keycaps in the modifiers to facilitate their location by touch

- Optimized for the Spanish language

- Usage of a visual layout as a mnemonic aid

&nbsp;
&nbsp;

# 🇪🇸 Descripción

Firmaware para [Teclado Corne](https://github.com/foostan/crkbd) especialmente diseñado para desarrolladores de software que utilizan macOS y Windows. 

Algunos de los conceptos que se han tenido en cuenta:

* Posibilidad de alternar entre sistemas operativos presionando ``CFG + O`` o ``M1 + OS``.
* Los atajos de teclas son independientes del sistema operativo: las mismas combinaciones de teclas realizan las mismas acciones ya estemos en macOS o en Windows (copiar, pegar, capturar la pantalla, apagar el ordenador, cambiar entre escritorios, reordenar las ventanas, etc ...).
* Posibilidad de alternar entre lenguajes: inglés / español. En español las teclas acentuadas se consiguen con una doble pulsación de la vocal en cuestión, por ejemplo: ``á = a + a``. Lo mismo sucede con la ``ñ = n + n``. Se puede alternarnar entre inglés y español presionando ``CFG + L``.
* Compatible con teclas y combinaciones de teclas dependientes del sistema operativo de manera sencilla. También soporta secuencias de carateres y salida de caracteres unicode personalizadas en función del OS.
* Las teclas de modificación pueden tener muchos comportamientos simultáneamente. Modificador pasivo: mantiene presionada una tecla mientras se mantengan presionada de manera independiente, pulsación normal cuando la tecla es accionada y liberada en un lapso corto de tiempo, modificador común cuando se mantiene pulsada al mismo tiempo que se presiona una segunda tecla. Las teclas de modificación se han representado con un color diferencial en el mapa de teclas.
* Se ha creado una tecla Hyper (y su correspondiente Bloqueo Hyper), para permitir atajos de teclado personalizables a nivel de aplicación. De esta manera ``hyper + i`` puede significar "indentar" en un editor de código, "invertir colores" en Photoshop, o poner un texto en "italica" en un procesador de texto. Las posibilidades son infinitas. Este tipo de comportamiento se puede conseguir en combiación con programas como [Karabiner-Elements](https://karabiner-elements.pqrs.org/).
* Soporte para temas visuales (``CFG + R``) y alertas luminosas empleando los LEDs. Se incluye un [editor de temas online](https://codepen.io/alvaro-prieto/full/gOLrwKm). 
* Tap dance avanzado: algunas teclas actúan de manera diferente cuando son presionadas varias veces seguidas en un lapso corto de tiempo. Compatible con teclas y combinaciones de teclas dependientes del sistema operativo, secuencias, y codigos extendidos uint16_t.
* Algunas teclas se han reubicado para mejorar la ergonomía. En el futuro es posible que poco a poco la distribución de teclas vaya pariéndose un poco más a la distribución Workman, pero he preferido hacer el salto gradual.
* Posibilidad de bloquear el teclado con password, útil para cuando te levantas unos minutos de tu puesto de trabajo. Presiona ``M6 + L`` o ``M6 + ☾`` si también quieres apagar la pantalla. Escribe "yo" para desbloquear el teclado.

&nbsp;
&nbsp;
## Configuración adicional

Algunas características requieren instalar y configurar aplicaciones de terceros; algunas no tienen equivalencia directa entre sistemas operativos. 

* Atajos de teclado por aplicación empleando la tecla hyper (Karabiner Elements).
* Funciones de pantalla partida en macOS (BetterSnapTool).
* Mover una aplicación a un escritorio en concreto.
* Algunas salidas especiales se han conseguido con aplicaciones de mapeo como Karabiner.
* En la configuración del sistema de macOS -> teclado:  desabilitar "Usar F1, F2, etc. como teclas de función".
* En Windows instalar WinCompose para dar soporte a las teclas unicode.
* La combinación de teclas para apagar el sistema operativo en Windows puede variar en función del idioma de tu sistema operativo. Actualmente compatible con versión en Español.
* La posibilidad de alternar entre escritorios en macOS requiere de 3 escritorios y activar los atajos en: teclado -> funciones rápidas -> mission control.
* Si estas experimentando problemas con caracteres como ``ª``, ``>`` y otras teclas que generan salidas inesperadas, comprueba la configuración regional de tu teclado en el sistema operativo. En Karabiner, selecciona ``Country code: 1`` para tu teclado Corne. La configuración regional en macOS ha de establecerse en``Español - ISO``.


&nbsp;
&nbsp;
## Notas para desarrolladores

Para más información acudir al archivo ``keymap.c``. Se han realizado algunas modificaciones mínimas en el código de QMK para poder compilar este keymap; dichos cambios han sido listados en la sección ``Settings``. 


