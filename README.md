# Pay 'N' Spray Color Picker

**Select your car color on Pay 'n' Spray - Grand Theft Auto San Andreas**  
**Selecione a cor do seu carro no Pay 'n' Spray - Grand Theft Auto San Andreas**

[![Build Status](https://github.com/Fabio3rs/Pay-n-Spray-color-picker/workflows/Build%20and%20publish%20release/badge.svg)](https://github.com/Fabio3rs/Pay-n-Spray-color-picker/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

![Pay-n-Spray-color-picker demonstration](https://4.bp.blogspot.com/-N-zR04MpfeI/WAAa-REN1iI/AAAAAAAAH5Q/pH6xj-zyxjMoOQdoG2q1Xs1Zuw9ESobZgCLcB/s400/m9p0nm.jpg)

## 📖 Description / Descrição

**English:**  
This .asi mod enables you to select custom colors for your car when painting it at Pay 'N' Spray locations in Grand Theft Auto San Andreas. Instead of getting random colors, you can now choose from a color palette to customize up to 4 different color layers of your vehicle.

**Português:**  
Este mod .asi permite que você selecione cores personalizadas para seu carro ao pintá-lo nos Pay 'N' Spray em Grand Theft Auto San Andreas. Em vez de obter cores aleatórias, agora você pode escolher de uma paleta de cores para customizar até 4 camadas de cores diferentes do seu veículo.

## ✨ Features / Características

- 🎨 **Interactive Color Selection** - Choose from a visual color grid
- 🚗 **Multi-layer Support** - Customize up to 4 different color layers
- 🌐 **Bilingual Interface** - English and Portuguese support
- ⚡ **Easy Controls** - Simple keyboard navigation
- 🔧 **Seamless Integration** - Works with existing Pay 'N' Spray mechanics

## 🎮 Controls / Controles

| Action / Ação | GTA SA Control / Controle do GTA SA | Default Key / Tecla Padrão |
|---------------|-------------------------------------|----------------------------|
| Select Color / Selecionar Cor | **Sprint** | **Left Shift** |
| Skip Color / Pular Cor | **Jump** | **Space** |
| Exit Without Changes / Sair Sem Alterações | **Enter Vehicle** | **F** or **Enter** |

**Note:** The mod uses your configured GTA SA controls. If you changed the default key bindings, use your custom keys.  
**Nota:** O mod usa seus controles configurados do GTA SA. Se você alterou as teclas padrão, use suas teclas personalizadas.

## 📥 Installation / Instalação

**English:**
1. Download the latest `paynspray.asi` file from the [Releases](https://github.com/Fabio3rs/Pay-n-Spray-color-picker/releases) page
2. Copy the file to your GTA San Andreas game folder (where `gta_sa.exe` is located)
3. Make sure you have an ASI Loader installed (like [Silent's ASI Loader](https://www.gtagarage.com/mods/show.php?id=21709))

**Português:**
1. Baixe o arquivo `paynspray.asi` mais recente da página de [Releases](https://github.com/Fabio3rs/Pay-n-Spray-color-picker/releases)
2. Copie o arquivo para sua pasta do jogo GTA San Andreas (onde está localizado o `gta_sa.exe`)
3. Certifique-se de ter um ASI Loader instalado (como o [Silent's ASI Loader](https://www.gtagarage.com/mods/show.php?id=21709))

## 🚀 Usage / Uso

**English:**
1. Drive your car to any Pay 'N' Spray location
2. Enter the garage as usual
3. A color selection panel will appear
4. Use arrow keys to navigate the color grid
5. Press **Sprint** key to select a color or **Jump** key to skip that color slot
6. Repeat for up to 4 color layers
7. Use **Enter Vehicle** key to exit without any changes
8. If you skip all colors, only repairs will be applied (no color change)
9. You can mix skip/select to change only specific color slots

**Português:**
1. Dirija seu carro para qualquer Pay 'N' Spray
2. Entre na garagem normalmente
3. Um painel de seleção de cores aparecerá
4. Use as setas do teclado para navegar na grade de cores
5. Pressione a tecla **Correr** para selecionar uma cor ou **Pular** para pular aquele slot de cor
6. Repita para até 4 camadas de cores
7. Use a tecla **Entrar no Veículo** para sair sem alterações
8. Se você pular todas as cores, apenas reparos serão aplicados (sem mudança de cor)
9. Você pode misturar pular/selecionar para alterar apenas slots de cor específicos

## 🔨 Building from Source / Compilando do Código

### Requirements / Requisitos
- CMake 3.16+
- MinGW-w64 (32-bit)
- Ninja build system (optional)

### Build Steps / Passos da Compilação

```bash
# Clone the repository / Clone o repositório
git clone --recursive https://github.com/Fabio3rs/Pay-n-Spray-color-picker.git
cd Pay-n-Spray-color-picker

# Create build directory / Crie o diretório de build
mkdir build && cd build

# Configure with CMake / Configure com CMake
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel

# Build the project / Compile o projeto
cmake --build . --config MinSizeRel --target all -j$(nproc)

# The output file will be: paynspray.asi
# O arquivo de saída será: paynspray.asi
```

## 🧪 Testing / Testes

```bash
cd build
ctest -j10 -C Release -T test --output-on-failure
```

## 📋 Requirements / Requisitos

- Grand Theft Auto San Andreas 1.0 US (compact exe also supported) or compatible / Grand Theft Auto San Andreas 1.0 US (exe compacto também suportado) ou compatível
- ASI Loader (Silent's ASI Loader recommended)
- Windows operating system

## 📄 License / Licença

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

Este projeto está licenciado sob a Licença MIT - veja o arquivo [LICENSE](LICENSE) para detalhes.

## 👨‍💻 Credits / Créditos

- **Original Author:** Fabio3rs
- **Published by:** Junior_Djjr (August 29, 2022)
- **Injector Library:** [thelink2012/injector](https://github.com/thelink2012/injector)

## 🔗 Links

- **Original MixMods Post:** https://www.mixmods.com.br/2022/08/sa-pay-n-spray-select-color/
- **GitHub Repository:** https://github.com/Fabio3rs/Pay-n-Spray-color-picker
- **Issues & Support:** https://github.com/Fabio3rs/Pay-n-Spray-color-picker/issues

## 🤝 Contributing / Contribuindo

Contributions are welcome! Please feel free to submit a Pull Request.

Contribuições são bem-vindas! Sinta-se à vontade para enviar um Pull Request.

