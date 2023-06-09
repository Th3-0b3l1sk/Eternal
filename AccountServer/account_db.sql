-- ----------------------------------------------------------------------\
-- - Create the database if it doesn't exist ----------------------------\
-- ----------------------------------------------------------------------\

IF NOT EXISTS(SELECT * FROM sys.databases WHERE name='e_account_db')
BEGIN
	CREATE DATABASE [e_account_db];
END

USE [e_account_db];


-- ----------------------------------------------------------------------\
-- - ACCOUNT TABLE ------------------------------------------------------\
-- ----------------------------------------------------------------------\
IF OBJECT_ID(N'accounts', N'U') iS NULL
BEGIN
	CREATE TABLE [accounts] (
	[_id]				INT				PRIMARY KEY IDENTITY(1,1),
	[_name]				VARCHAR(16)		UNIQUE NOT NULL DEFAULT '',
	[_password]			VARCHAR(16)		NOT NULL DEFAULT '',
	[_type]				TINYINT			NOT NULL DEFAULT 02,
	[_creation_time]	BIGINT			NOT NULL DEFAULT 0,
	[_online]			TINYINT			NOT NULL DEFAULT 0,
	[_last_ip]			VARCHAR(15)		NOT NULL DEFAULT '127.0.0.1'
	)
END


-- ----------------------------------------------------------------------\
-- - Dev Accounts -------------------------------------------------------\
-- ----------------------------------------------------------------------\
-- Encrypt passwords before saving?
INSERT INTO [accounts] (_name, _password, _type, _creation_time, _online, _last_ip) VALUES 
	('0b3l1sk', '1', 01, 20080505, 0, '127.0.0.1');
