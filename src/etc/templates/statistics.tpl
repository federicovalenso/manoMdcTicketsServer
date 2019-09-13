<!DOCTYPE html>

<html lang="ru">

<head>
	<title>Статистика талонов в регистратуру МАНО "ЛДЦ"</title>
	<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">
	<meta http-equiv="content-style-type" content="text/css">

	<link rel="icon" href="/img/logo.png" type="image/x-icon">
  <link href="/css/app.css" rel="stylesheet">
</head>

<body>

	<nav class="navbar navbar-expand-lg navbar-light bg-light">
		<div class="container">
      <a class="navbar-brand" href="/"><img class="logo" src="/img/logo.svg"></a>
			<button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarSupportedContent"
				aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
				<span class="navbar-toggler-icon"></span>
			</button>
	</nav>

	<div class="container">
		<main id="app">
			<b-card no-body>
				<b-tabs card>
					<b-tab title="По талонам за период" active>
						<tickets-chart></tickets-chart>
					</b-tab>
					<b-tab title="По регистраторам"><p>В процессе...</p></b-tab>
				</b-tabs>
			</b-card>
		</main>
	</div>

  <script src="/js/manifest.js"></script>
  <script src="/js/vendor.js"></script>
  <script src="/js/app.js"></script>
</body>

</html>
