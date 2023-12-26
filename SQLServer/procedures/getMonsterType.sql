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
-- Create date: <Tue Dec 26 2023>
-- Description:	<loads Monsters info>
-- =============================================
CREATE PROCEDURE get_monster_type
	
AS
BEGIN
	
	SELECT * 
	FROM [dbo].monstertype;

END
GO
