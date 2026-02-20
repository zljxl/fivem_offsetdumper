# 🎮 FiveM Offset Dumper

Ferramenta para **extração automática de offsets atualizados do FiveM**.

Desenvolvido por **Pedro Guarconi** e colaborador.

---

## 📖 Visão geral

O FiveM Offset Dumper automatiza a obtenção de offsets utilizados pelo FiveM, eliminando a necessidade de análise manual a cada atualização.

A ferramenta analisa o processo em execução e gera uma lista organizada de offsets prontos para uso.

---

## ✨ Recursos

- Dump automático de offsets
- Compatível com atualizações recentes do FiveM
- Estrutura de saída limpa e organizada
- Execução rápida e leve
- Fácil integração com outros projetos

---

## 🧱 Tecnologias

- **C++**
- **Windows API**
- Técnicas de análise de memória

---

## 📦 Dependências

O projeto utiliza:

- **MinHook** — API hooking leve e eficiente  
- **ImGui** — API Grafica
- 
As dependências ja estao incluidas.

---

## 🖥️ Requisitos

- Windows 10 ou superior
- **Visual Studio 2022**
- MSVC Toolset v143
- Compilação em **Release x64**

---

## 🚀 Build & Uso

### 1. Clonar o repositório

git clone https://github.com/seuusuario/fivem-offset-dumper.git

### 2. Abrir no Visual Studio

Abra o arquivo `.sln` utilizando o **Visual Studio 2022**.

### 3. Compilar

Selecione:

Release | x64

e compile o projeto.

### 4. Renomear `adhesive.dll`

Abrir o diretorio do FiveM e renomear a `adhesive.dll` para `adhesive.dll.bkp`

### 5. Executar

OffsetDumper.exe

### 6. Resultado

Os offsets serão gerados automaticamente em:

offsets.txt

---

## 📄 Exemplo de saída

World: 0x2476F50  
ReplayInterface: 0x1F42028  
Viewport: 0x1F9A3D0  
LocalPlayer: 0x8  

---

## ⚠️ Aviso

Este projeto é fornecido **apenas para fins educacionais e de pesquisa**.

Os autores não se responsabilizam por qualquer uso indevido.


## 👨‍💻 Autores

**Pedro Guarconi**  
**Gabriel Cancella**
