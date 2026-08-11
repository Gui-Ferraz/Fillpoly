# Fillpoly

Projeto desenvolvido para a disciplina de Computação Gráfica.
Aluno: Guilherme Marchiotti Ferraz
Disponível em: https://github.com/Gui-Ferraz/Fillpoly

## Requisitos

- Visual Studio 2022 (64 bit) - Versão 17.14.37 (Julho 2026)
- Desktop development with C++
- C++17
- SFML 3.1.0

## Compilação

1. Abra 'Fillpoly.sln' no Visual Studio 2022.
2. Selecione 'x64' como plataforma.
3. Selecione 'Debug' ou 'Release'.
4. Compile a solução (Build -> Build Solution).

## SFML

Os arquivos de desenvolvimento da SFML 3.1.0 ('include' e 'lib')
estão incluídos na pasta 'SFML' do projeto.

As DLLs necessárias para execução devem ser obtidas a partir
da distribuição oficial da SFML 3.1.0 e colocadas no diretório
do executável.

Link de download: https://www.sfml-dev.org/download/sfml/3.1.0/
Plataforma: 64-bit Visual C++ 17 (2022)

- Extraia os arquivos de 'SFML-3.1.0-windows-vc17-64-bit.zip'
- Vá até o diretório SFML-3.1.0\bin\
- Caso tenha compilado em modo Release, copie as DLLs:
	sfml-graphics-3.dll
	sfml-system-3.dll
	sfml-window-3.dll
E cole no diretório Fillpoly\x64\Release\
- Caso tenha compilado em modo Debug, copie as DLLs:
	sfml-graphics-d-3.dll
	sfml-system-d-3.dll
	sfml-window-d-3.dll
E cole no diretório Fillpoly\x64\Debug\

Fillpoly/
├── ...
└── x64/
    ├── Debug/
    │   ├── Assets/
    │   ├── Fillpoly.exe
    │   ├── sfml-graphics-d-3.dll
    │   ├── sfml-window-d-3.dll
    │   └── sfml-system-d-3.dll
    │
    └── Release/
        ├── Assets/
        ├── Fillpoly.exe
        ├── sfml-graphics-3.dll
        ├── sfml-window-3.dll
        └── sfml-system-3.dll

## Estrutura

Fillpoly/
│
├── Fillpoly.sln
├── README.txt
├── x64/
│
└── Fillpoly/
    │
    ├── Fillpoly.vcxproj
    ├── Fillpoly.vcxproj.filters
    │
    ├── Assets/
    │   └── DejaVuSans.ttf
    │
    ├── SFML/
    │   ├── include/
    │   │   ├── mbedtls/
    │   │   ├── psa/
    │   │   ├── SFML/
    │   │   └── ...
    │   │
    │   └── lib/
    │       ├── sfml-graphics.lib
    │       ├── sfml-graphics-d.lib
    │       ├── sfml-system.lib
    │       ├── sfml-system-d.lib
    │       ├── sfml-window.lib
    │       └── sfml-window-d.lib
    │
    ├── Color.hpp
    ├── Fillpoly.hpp
    ├── Fillpoly.vcsproj
    ├── Fillpoly.vcsproj.filters
    ├── Framebuffer.cpp
    ├── Framebuffer.hpp
    ├── Menu.cpp
    ├── Menu.hpp
    ├── Point.hpp
    ├── Polygon.hpp
    ├── Rasterizer.cpp
    ├── Rasterizer.hpp
    ├── Renderer.cpp
    └── Renderer.hpp

- '*.cpp' / '.hpp'— código-fonte do projeto
- 'Assets/'— recursos utilizados pelo programa
- 'SFML/' — arquivos de desenvolvimento da SFML 3.1.0
- *.vcxproj*' — arquivos de configuração do visual studio