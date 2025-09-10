#!/bin/bash
# build.sh - For Linux builds

set -ex

# Create the target directories
mkdir -p $PREFIX/bin
mkdir -p $PREFIX

# Copy the shared library and plugin file
cp bin/libplugify-plugin-configs.so $PREFIX/bin/
cp plugify-plugin-configs.pplugin $PREFIX/

# Set proper permissions
chmod 755 $PREFIX/bin/libplugify-plugin-configs.so
chmod 644 $PREFIX/plugify-plugin-configs.pplugin

# Create activation scripts for proper library path
mkdir -p $PREFIX/etc/conda/activate.d
mkdir -p $PREFIX/etc/conda/deactivate.d

cat > $PREFIX/etc/conda/activate.d/plugify-plugin-configs.sh << EOF
#!/bin/bash
export PLUGIFY_CONFIGS_PLUGIN_PATH="\${CONDA_PREFIX}:\${PLUGIFY_CONFIGS_PLUGIN_PATH}"
EOF

cat > $PREFIX/etc/conda/deactivate.d/plugify-plugin-configs.sh << EOF
#!/bin/bash
export PLUGIFY_CONFIGS_PLUGIN_PATH="\${PLUGIFY_CONFIGS_PLUGIN_PATH//\${CONDA_PREFIX}:/}"
EOF

chmod +x $PREFIX/etc/conda/activate.d/plugify-plugin-configs.sh
chmod +x $PREFIX/etc/conda/deactivate.d/plugify-plugin-configs.sh