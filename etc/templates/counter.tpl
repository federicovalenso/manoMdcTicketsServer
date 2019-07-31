<html lang="ru">
<head>
    <title>Счётчик талонов</title>
</head>
<body>
    <form method="POST" action="/counter">
        <div>
            <label>Текущее значение</label>
            <input type="number" value="{count}" readonly>
        </div>
        <div>
            <input type="submit" value="Сбросить">
        </div>
    </form>
</body>
</html>
