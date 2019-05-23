SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+08:00";

-- --------------------------------------------------------

CREATE DATABASE clinic_queue;
USE clinic_queue;

--
-- Структура таблицы `ticketActions`
--

CREATE TABLE `ticketActions` (
  `id` tinyint(3) UNSIGNED NOT NULL,
  `action_name` varchar(10) NOT NULL,
  `prefix` char(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Дамп данных таблицы `ticketActions`
--

INSERT INTO `ticketActions` (`id`, `action_name`, `prefix`) VALUES
(1, 'book', 'Б'),
(2, 'disp', 'Д'),
(3, 'receipt', 'Р'),
(4, 'emergency', 'О'),
(5, 'report', 'С'),
(6, 'vaccine', 'В'),
(7, 'other', 'П');

-- --------------------------------------------------------

--
-- Структура таблицы `tickets`
--

CREATE TABLE `tickets` (
  `id` int(10) UNSIGNED NOT NULL,
  `number_by_action` smallint(5) UNSIGNED DEFAULT NULL,
  `action_id` tinyint(3) UNSIGNED NOT NULL,
  `user_id` smallint(6) DEFAULT NULL,
  `created_at` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `updated_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `on_service` tinyint(1) NOT NULL DEFAULT '0',
  `is_done` tinyint(1) NOT NULL DEFAULT '0',
  `is_voiced` tinyint(1) NOT NULL DEFAULT '0',
  `is_manual` tinyint(1) NOT NULL DEFAULT '0',
  `window` tinyint(3) UNSIGNED DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Структура таблицы `users`
--

CREATE TABLE `users` (
  `id` smallint(6) NOT NULL,
  `user_name` varchar(90) NOT NULL,
  `full_name` text,
  `password` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Дамп данных таблицы `users`
--

INSERT INTO `users` (`user_name`, `full_name`, `password`) VALUES
('Администратор', 'Администратор', 'c97c4bea8165c95b030a1eaf392aabd80a88b364d89af1970469f8e85ef1c70e2154feddab81ba3e933d8df03c828d573b1386c84856bdede7ee5cefe339652c');

--
-- Индексы сохранённых таблиц
--

--
-- Индексы таблицы `ticketActions`
--
ALTER TABLE `ticketActions`
  ADD PRIMARY KEY (`id`);

--
-- Индексы таблицы `tickets`
--
ALTER TABLE `tickets`
  ADD PRIMARY KEY (`id`),
  ADD KEY `employee_id` (`user_id`),
  ADD KEY `done_at` (`is_done`),
  ADD KEY `action_id` (`action_id`),
  ADD KEY `number_by_action` (`number_by_action`);

--
-- Индексы таблицы `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `name_2` (`user_name`),
  ADD UNIQUE KEY `id` (`id`),
  ADD KEY `name` (`user_name`);

--
-- AUTO_INCREMENT для сохранённых таблиц
--

--
-- AUTO_INCREMENT для таблицы `ticketActions`
--
ALTER TABLE `ticketActions`
  MODIFY `id` tinyint(3) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;

--
-- AUTO_INCREMENT для таблицы `tickets`
--
ALTER TABLE `tickets`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT для таблицы `users`
--
ALTER TABLE `users`
  MODIFY `id` smallint(6) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=21;

--
-- Ограничения внешнего ключа сохраненных таблиц
--

--
-- Ограничения внешнего ключа таблицы `tickets`
--
ALTER TABLE `tickets`
  ADD CONSTRAINT `tickets_ibfk_1` FOREIGN KEY (`action_id`) REFERENCES `ticketActions` (`id`),
  ADD CONSTRAINT `tickets_ibfk_2` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`);

--
-- Триггеры `tickets`
--
CREATE TRIGGER `generate_number_by_action` BEFORE INSERT ON `tickets` FOR EACH ROW SET NEW.number_by_action = (SELECT COUNT(*) FROM tickets WHERE DATE(created_at) = CURDATE() AND action_id = NEW.action_id) + 1;
COMMIT;
