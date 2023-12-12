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

-- =============================================
-- Author:		<Mahmoud Abdullah>
-- Create date: <Sat Dec 9 2023>
-- Description:	<retrieves the items that a player holds which includes the inventory and the equiped items>
-- =============================================
CREATE PROCEDURE get_player_own_items (
	-- Add the parameters for the stored procedure here
	@player_id INT 
	)
	
AS
BEGIN
	SELECT * 
	FROM [dbo].[items] 
	WHERE _player_id=@player_id;
END
GO
