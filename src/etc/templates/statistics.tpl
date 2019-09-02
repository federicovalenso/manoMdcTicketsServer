<!DOCTYPE html>

<html lang="ru">

<head>
	<title>Статистика талонов в регистратуру МАНО "ЛДЦ"</title>
	<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">
	<meta http-equiv="content-style-type" content="text/css">

	<link rel="icon" href="img/logo.png" type="image/x-icon">
  <link href="/css/app.css" rel="stylesheet">
</head>

<body>

	<nav class="navbar navbar-expand-lg navbar-light bg-light">
		<div class="container">
			<a class="navbar-brand" href="/"><img class="logo" src="img/logo.png"></a>
			<button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarSupportedContent"
				aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
				<span class="navbar-toggler-icon"></span>
			</button>
			<div class="collapse navbar-collapse" id="navbarSupportedContent">
				<ul class="navbar-nav mr-auto">
					<li class="nav-item">
						<a class="nav-link" href="/statistics/count">По талонам за период</a>
					</li>
					<li class="nav-item">
						<a class="nav-link" href="/statistics/registrar">По регистраторам</a>
					</li>
				</ul>
			</div>
	</nav>

	<div class="container">
		<main id="app">
			<chart-component></chart-component>
		</main>
	</div>

  <script src="/js/manifest.js"></script>
  <script src="/js/vendor.js"></script>
  <script src="/js/app.js"></script>
</body>

</html>
