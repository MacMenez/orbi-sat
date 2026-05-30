/* Script SQL para criação do banco de dados orbi_server e da tabela dadosSatelites */

CREATE DATABASE orbi_server;
USE orbi_server;
SHOW DATABASES;
SHOW TABLES;

SELECT * FROM dadosSatelites ORDER BY id DESC;

DROP TABLE IF EXISTS dadosSatelites;

/* Criação da tabela dadosSatelites que será criada no código em Python (não será limitada apenas ao satélite, mas iniciará com ele)*/

CREATE TABLE dadosSatelites (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nome VARCHAR(255) NOT NULL,
    tipo VARCHAR(255) NOT NULL,
    data_lancamento DATE NOT NULL,
    peso FLOAT NOT NULL,
    altura FLOAT NOT NULL
);

/* Comando que será utilizado no código em Python */
INSERT INTO dadosSatelites (nome, tipo, data_lancamento, peso, altura) VALUES ('Satélite A', 'Comunicação', '2020-01-01', 500.0, 2.5);
INSERT INTO dadosSatelites (nome, tipo, data_lancamento, peso, altura) VALUES ('Satélite B', 'Observação', '2021-06-15', 300.0, 1.8);
VALUES ('Satélite C', 'Navegação', '2019-11-30', 400.0, 2.0);