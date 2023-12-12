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
-- Description:	<Retrieves the registerd player names>
-- =============================================
CREATE PROCEDURE get_player_id_by_name
	@player_name VARCHAR(15) 
AS
BEGIN

	SELECT _account_id, _identity 
	FROM [dbo].[users] 
	WHERE @player_name=_name;


END
GO
