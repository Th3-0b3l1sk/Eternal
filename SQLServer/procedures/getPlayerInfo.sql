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
-- Create date: <Sat Dec 9 2023>
-- Description:	<retrieves the ingame player info (stats)>
-- =============================================
CREATE PROCEDURE get_player_info
	(
	@player_id INT
	)
	
AS
BEGIN
	SELECT * FROM [dbo].[users] WHERE _identity=@player_id;
END
GO
