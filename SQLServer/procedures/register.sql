-- ================================================
-- Template generated from Template Explorer using:
-- Create Procedure (New Menu).SQL
--
-- Use the Specify Values for Template Parameters 
-- command (Ctrl-Shift-M) to fill in the parameter 
-- values below.
--
-- This block of comments will not be included in
-- the definition of the procedure.
-- ================================================
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Mahmoud Abdullah>
-- Create date: <Fri, December 8, 2023>
-- Description:	<Registers a new user to the account database>
-- =============================================
CREATE PROCEDURE register (
	-- Add the parameters for the stored procedure here
	@name     varchar(16),
	@password varchar(16),
	@type     tinyint      = 2,	-- user
	@last_ip  varchar(15)  = '127.0.0.1'
	)
AS
BEGIN
	
	INSERT INTO [dbo].[accounts] (_name, _password, _last_ip, _type, _creation_time) 
	VALUES (
	@name,
	@password, 
	@last_ip,
	@type,
	CURRENT_TIMESTAMP);

END
GO
