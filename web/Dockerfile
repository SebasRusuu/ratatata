# Dockerfile para a aplicação React
FROM node:18

# Definir o diretório de trabalho no contêiner
WORKDIR /web

# Copiar package.json e package-lock.json
COPY package*.json ./

# Instalar dependências
RUN npm install --legacy-peer-deps

# Copiar o restante do código da aplicação
COPY . .

# Compilar a aplicação React
CMD ["npm", "run", "dev", "--", "--host", "--port", "3000"]

EXPOSE 3000