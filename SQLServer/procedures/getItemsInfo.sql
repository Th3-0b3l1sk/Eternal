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
-- Create date: <Sun Dec 10 2023>
-- Description:	<gets all of the in-games items' info>
-- =============================================
CREATE PROCEDURE get_items_info
AS
BEGIN
	SELECT * 
	FROM [dbo].[itemtype];
END
GO
