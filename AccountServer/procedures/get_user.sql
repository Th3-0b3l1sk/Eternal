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
-- Author:		Mahmoud Abdullah
-- Create date: Fri, Dec 8, 2023
-- Description:	retrieves a user's info 
-- =============================================
CREATE PROCEDURE get_account_info (
	-- Add the parameters for the stored procedure here
	@name     varchar(16),
	@id       int         OUTPUT,
	@password varchar(16) OUTPUT,
	@type	  tinyint     OUTPUT,
	@last_ip  VARCHAR(15) OUTPUT,
	@isOnline TINYINT     OUTPUT
	)
AS
BEGIN

	SELECT @id=_id, @password=_password, @type=_type, @last_ip=_last_ip, @isOnline=_online FROM [dbo].[accounts] 
	WHERE _name=@name;

END
GO


