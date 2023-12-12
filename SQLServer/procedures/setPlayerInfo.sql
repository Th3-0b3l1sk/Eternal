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
-- Create date: <Tuesday Dec 12 2023>
-- Description:	<Sets the player's stats for new registeration>
-- =============================================
CREATE PROCEDURE set_player_info(
	@account_id INT,
	@identity   INT,
	@name	    VARCHAR(15),
	@mate		VARCHAR(15),
	@lookface	INT,
	@hair		SMALLINT,
	@money		INT,
	@money_save INT,
	@cps		INT,
	@level		TINYINT,
	@exp		BIGINT,
	@force		SMALLINT,
	@dex		SMALLINT,
	@health		SMALLINT,
	@soul		SMALLINT,
	@add_points	SMALLINT,
	@life		SMALLINT,
	@mana		SMALLINT,
	@profession	TINYINT,
	@pk			SMALLINT,
	@virtue		INT,
	@nobility	TINYINT,
	@rebirth	TINYINT,
	@syn_id		SMALLINT,
	@record_map	INT,
	@record_x	SMALLINT,
	@record_y	SMALLINT,
	@last_login	BIGINT
	)
	
AS
BEGIN

	INSERT INTO [dbo].[users]
	VALUES 
	(@account_id, @identity, @name, @mate, @lookface, @hair, @money,@money_save, @cps,
	@level, @exp, @force, @dex, @health, @soul, @add_points, @life, @mana, @profession, @pk, 
	@virtue, @nobility, @rebirth, @syn_id, @record_map, @record_x, @record_y, @last_login);

END
GO
