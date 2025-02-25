#!/bin/bash

# Verifica se o usuário tem permissão de root
if [ "$EUID" -ne 0 ]; then
  echo "Por favor, execute como root (use sudo)"
  exit 1
fi

echo "🔧 Configurando interface virtual CAN (vcan0)..."

# Carrega o módulo vcan
echo "🔹 Carregando módulo vcan..."
modprobe vcan

# Verifica se a interface já existe
if ip link show vcan0 &> /dev/null; then
  echo "✅ Interface vcan0 já existe."
else
  # Adiciona a interface vcan0
  echo "🔹 Criando interface vcan0..."
  ip link add dev vcan0 type vcan

  # Ativa a interface vcan0
  echo "🔹 Ativando interface vcan0..."
  ip link set up vcan0

  echo "✅ Interface vcan0 configurada com sucesso!"
fi

# Exibir interfaces VCAN ativas
echo "📜 Listando interfaces VCAN ativas:"
ip link show type vcan
