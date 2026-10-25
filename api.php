<?php
// Configurações do banco de dados
$host = 'localhost';
$dbname = 'DATABASE_NAME';
$user = 'USUARIO_DATABASE';
$pass = 'SUA_SENHA';

try {
    $pdo = new PDO("mysql:host=$host;dbname=$dbname;charset=utf8", $user, $pass);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch (PDOException $e) {
    die("Erro na conexão: " . $e->getMessage());
}

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $node_id = $_POST['node_id'] ?? null;
    $umidade = $_POST['umidade'] ?? null;

    if ($node_id && $umidade !== null) {
        $stmt = $pdo->prepare("INSERT INTO leituras (node_id, umidade, data_hora) VALUES (:node_id, :umidade, NOW())");
        $stmt->bindParam(':node_id', $node_id);
        $stmt->bindParam(':umidade', $umidade);
        
        if ($stmt->execute()) {
            http_response_code(200);
            echo "Dados salvos com sucesso";
        } else {
            http_response_code(500);
            echo "Erro ao salvar no banco";
        }
    } else {
        http_response_code(400);
        echo "Dados incompletos";
    }
}
?>