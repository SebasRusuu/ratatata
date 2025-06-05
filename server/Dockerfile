FROM node:18-alpine

# Instala dependências do Node-RED
RUN npm install -g --unsafe-perm node-red

# Cria diretório de trabalho
WORKDIR /data

# Expõe porta e inicia
EXPOSE 1880
CMD ["node-red", "--userDir", "/data"]
