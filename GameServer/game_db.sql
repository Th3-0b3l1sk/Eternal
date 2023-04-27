-- ----------------------------------------------------------------------\
-- - Create the database if it doesn't exist ----------------------------\
-- ----------------------------------------------------------------------\

IF NOT EXISTS(SELECT * FROM sys.databases WHERE name='e_game_db')
BEGIN
	CREATE DATABASE [e_game_db];
END

USE [e_game_db];


-- ----------------------------------------------------------------------\
-- - USERS TABLE --------------------------------------------------------\
-- ----------------------------------------------------------------------\
IF OBJECT_ID(N'users', N'U') IS NULL
BEGIN 
	CREATE TABLE [users] (
	[_account_id]	 INT NOT NULL			DEFAULT 0,
	[_identity]		 INT IDENTITY(1000000, 1) NOT NULL,
	[_name]			 VARCHAR(15) NOT NULL	DEFAULT 'UNKNOWN',
	[_mate]			 VARCHAR(15) NOT NULL	DEFAULT 'None',
	[_lookface]		 INT NOT NULL			DEFAULT 0,
	[_hair]			 SMALLINT NOT NULL		DEFAULT 310,
	[_money]		 INT NOT NULL			DEFAULT 500,
	[_money_saved]	 INT NOT NULL			DEFAULT 0,
	[_cps]			 INT NOT NULL			DEFAULT 1000,
	[_level]		 TINYINT NOT NULL		DEFAULT 1,
	[_exp]			 BIGINT NOT NULL		DEFAULT 0,
	[_force]		 SMALLINT NOT NULL		DEFAULT 0,
	[_dexterity]	 SMALLINT NOT NULL		DEFAULT 0,
	[_health]		 SMALLINT NOT NULL		DEFAULT 0,
	[_soul]			 SMALLINT NOT NULL		DEFAULT 0,
	[_add_points]	 SMALLINT NOT NULL		DEFAULT 0,
	[_life]			 SMALLINT NOT NULL		DEFAULT 1,
	[_mana]			 SMALLINT NOT NULL		DEFAULT 0,
	[_profession]	 TINYINT NOT NULL		DEFAULT 0,
	[_pk]			 SMALLINT NOT NULL		DEFAULT 0,
	[_virtue]		 INT NOT NULL			DEFAULT 0,
	[_nobility]		 TINYINT NOT NULL		DEFAULT 0,
	[_rebirth]		 TINYINT NOT NULL		DEFAULT 0,
	[_syndicate_id]	 SMALLINT NOT NULL		DEFAULT 0,
	[_record_map]	 INT NOT NULL			DEFAULT 1010,
	[_record_x]		 SMALLINT NOT NULL		DEFAULT 31,
	[_record_y]		 SMALLINT NOT NULL		DEFAULT 83,
	[_last_login]	 BIGINT NOT NULL		DEFAULT 20080505
	);
END
-- ----------------------------------------------------------------------\
-- - USERS Accounts -----------------------------------------------------\
-- ----------------------------------------------------------------------\
SET IDENTITY_INSERT [users] ON;
INSERT INTO [users] 
	([_account_id], [_identity], [_name], [_mate], [_lookface], [_hair], [_money], [_money_saved], [_cps], 
	[_level], [_exp], [_force], [_dexterity], [_health], [_soul], [_add_points], [_life], [_mana], [_profession], 
	[_pk], [_virtue], [_nobility], [_rebirth], [_syndicate_id], [_record_map], [_record_x], [_record_y], [_last_login] 
	) VALUES
(1, 1000001, 'Mahmoud[GM]', 'None', 1011004, 310, 1000000, 0, 25000, 137, 0, 180, 45, 120, 26, 0, 150, 100, 15, 30, 0, 0, 2, 0000, 1036, 0198, 0186, 0);

