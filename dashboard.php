<?php
$host = 'localhost';
$dbname = 'DATABASE_NAME';
$user = 'USUARIO_DATABASE';
$pass = 'SUA_SENHA';

$pdo = new PDO(mysqlhost=$host;dbname=$dbname;charset=utf8, $user, $pass);
 Busca as últimas 20 leituras para o gráfico
$stmt = $pdo-query(SELECT umidade, DATE_FORMAT(data_hora, '%H%i%s') as hora FROM leituras ORDER BY data_hora DESC LIMIT 20);
$dados = $stmt-fetchAll(PDOFETCH_ASSOC);
$dados = array_reverse($dados);  Inverte para ordem cronológica normal no gráfico

$labels = [];
$valores = [];
foreach ($dados as $linha) {
    $labels[] = $linha['hora'];
    $valores[] = $linha['umidade'];
}

!DOCTYPE html
html lang=pt-BR
head
    meta charset=UTF-8
    titleMonitoramento de Umidadetitle
    script src=httpscdn.jsdelivr.netnpmchart.jsscript
    style
        body { font-family Arial, sans-serif; text-align center; background-color #f4f4f9; }
        .chart-container { width 80%; margin auto; background white; padding 20px; border-radius 8px; box-shadow 0 4px 8px rgba(0,0,0,0.1); }
    style
head
body
    h2Dashboard de Umidade do Soloh2
    div class=chart-container
        canvas id=umidadeChartcanvas
    div

    script
        const ctx = document.getElementById('umidadeChart').getContext('2d');
        const umidadeChart = new Chart(ctx, {
            type 'line',
            data {
                labels php echo json_encode($labels); ,
                datasets [{
                    label 'Umidade do Solo (%)',
                    data php echo json_encode($valores); ,
                    borderColor 'rgba(54, 162, 235, 1)',
                    backgroundColor 'rgba(54, 162, 235, 0.2)',
                    borderWidth 2,
                    fill true,
                    tension 0.3
                }]
            },
            options {
                scales {
                    y { beginAtZero true, max 100 }
                }
            }
        });
        
         Atualiza a página a cada 10 segundos
        setTimeout(() = { location.reload(); }, 10000);
    script
body
html