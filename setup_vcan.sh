#!/bin/bash

# Checks if the user has root permissions
if [ "$EUID" -ne 0 ]; then
  echo "Please run as root (use sudo)"
  exit 1
fi

echo "🔧 Configuring virtual CAN interface (vcan0)..."

# Loads the vcan module
echo "🔹 Loading vcan module..."
modprobe vcan

# Checks if the interface already exists
if ip link show vcan0 &> /dev/null; then
  echo "✅ Interface vcan0 already exists."
else
  # Adds the vcan0 interface
  echo "🔹 Creating vcan0 interface..."
  ip link add dev vcan0 type vcan

  # Activates the vcan0 interface
  echo "🔹 Enabling vcan0 interface..."
  ip link set up vcan0

  echo "✅ Interface vcan0 successfully configured!"
fi

# Display active VCAN interfaces
echo "📜 Listing active VCAN interfaces:"
ip link show type vcan
