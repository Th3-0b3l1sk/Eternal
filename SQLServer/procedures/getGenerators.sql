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
-- Create date: <Wed Dec 27 2023>
-- Description:	<get monster generators>
-- =============================================
CREATE PROCEDURE get_generators
AS
BEGIN

	SELECT *
	FROM [dbo].[generators];

END
GO
