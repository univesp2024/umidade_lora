CREATE DATABASE IF NOT EXISTS monitoramento_solo DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE monitoramento_solo;

CREATE TABLE IF NOT EXISTS leituras (
    id INT AUTO_INCREMENT PRIMARY KEY,
    node_id VARCHAR(50) NOT NULL,
    umidade INT NOT NULL,
    data_hora DATETIME DEFAULT CURRENT_TIMESTAMP
);

