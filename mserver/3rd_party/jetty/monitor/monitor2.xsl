<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:output method="html" version="1.0" encoding="UTF-8" indent="yes"/>
	<xsl:template match="/mars_monitor">

		<table class="treeTable"  style="font-size:20px;">

		<thead>

			<tr>

				<th width="200px;">DataBase</th>
				
				<th width="380px;">NJDC DB Sync Delay</th>
				
				<th width="380px;">IAD1 DB Sync Delay</th>

			</tr>

		</thead>

		<tbody>

			<tr id="db">

				<td><xsl:value-of select="@db"/></td>
				
				<td id="dbNjdc"><xsl:value-of select="@db-njdc"/></td>
				
				<td id="dbIad1"><xsl:value-of select="@db-iad1"/></td>

			</tr>

		</tbody>

		</table>

		<table class="treeTable">
		
			<thead>

				<tr style="text-align:left;">
					<td style="width:160px;"></td>
					<td style="width:90px; text-align:right;">Queue</td>
					<td style="width:50px;"></td>
					<td style="width:50px;">Input</td>
					<td style="width:50px;"></td>
					<td style="width:50px;">VScan</td>
					<td style="width:50px;"></td>
					<td style="width:50px;">PAFI</td>
					<td style="width:50px;"></td>
					<td style="width:50px;">MSA</td>
					<td style="width:50px;"></td>
					<td style="width:50px;">MDA</td>
					<td style="width:50px;">Report</td>
					<td style="width:70px; text-align:right;">End</td>
				</tr>
				
			</thead>

			<tbody>	
			<xsl:for-each select="process">
				
					<tr style="text-align:right;">
					
						<td><xsl:value-of select="@name"/></td>
						
						<td>
							<xsl:for-each select="nodes">
								
								<xsl:if test="@module = 'input' and @type = 'queue'">
									<xsl:choose>
										<xsl:when test="@status=  'ok'">
											<font style="color:green;"><xsl:value-of select="@items"/></font>
										</xsl:when>
		
										<xsl:when test="@status = 'problem'">
											<font style="color:#993300;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:when test="@status = 'error'">
											<font style="color:red;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:otherwise>
											<font><xsl:value-of select="@items"/></font>
										</xsl:otherwise>
										
									</xsl:choose>
								</xsl:if>
								
							</xsl:for-each>
						</td>
						
						<td>	
						<xsl:for-each select="nodes">
							
							<xsl:if test="@module = 'input' and @type = 'work'">
								<xsl:choose>
										<xsl:when test="@status=  'ok'">
											<font style="color:green;"><xsl:value-of select="@items"/></font>
										</xsl:when>
		
										<xsl:when test="@status = 'problem'">
											<font style="color:#993300;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:when test="@status = 'error'">
											<font style="color:red;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:otherwise>
											<font><xsl:value-of select="@items"/></font>
										</xsl:otherwise>
										
									</xsl:choose>
							</xsl:if>
							
						</xsl:for-each>
						</td>
						
						<td>
						<xsl:for-each select="nodes">
							
							<xsl:if test="@module = 'init'">
								<xsl:choose>
										<xsl:when test="@status=  'ok'">
											<font style="color:green;"><xsl:value-of select="@items"/></font>
										</xsl:when>
		
										<xsl:when test="@status = 'problem'">
											<font style="color:#993300;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:when test="@status = 'error'">
											<font style="color:red;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:otherwise>
											<font><xsl:value-of select="@items"/></font>
										</xsl:otherwise>
										
									</xsl:choose>
							</xsl:if>
							
						</xsl:for-each>
						</td>
						
						<td>
						<xsl:for-each select="nodes">
							
							<xsl:if test="@module = 'vscan' and @type = 'work'">
								<xsl:choose>
										<xsl:when test="@status=  'ok'">
											<font style="color:green;"><xsl:value-of select="@items"/></font>
										</xsl:when>
		
										<xsl:when test="@status = 'problem'">
											<font style="color:#993300;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:when test="@status = 'error'">
											<font style="color:red;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:otherwise>
											<font><xsl:value-of select="@items"/></font>
										</xsl:otherwise>
										
									</xsl:choose>
							</xsl:if>
							
						</xsl:for-each>
						</td>
						
						<td>
						<xsl:for-each select="nodes">
							
							<xsl:if test="@module = 'vscan' and @type = 'queue'">
								<xsl:choose>
										<xsl:when test="@status=  'ok'">
											<font style="color:green;"><xsl:value-of select="@items"/></font>
										</xsl:when>
		
										<xsl:when test="@status = 'problem'">
											<font style="color:#993300;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:when test="@status = 'error'">
											<font style="color:red;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:otherwise>
											<font><xsl:value-of select="@items"/></font>
										</xsl:otherwise>
										
									</xsl:choose>
							</xsl:if>
							
						</xsl:for-each>
						</td>
						
						<td>
						<xsl:for-each select="nodes">
							
							<xsl:if test="@module = 'new_pafiscan' and @type = 'work'">
								<xsl:choose>
										<xsl:when test="@status=  'ok'">
											<font style="color:green;"><xsl:value-of select="@items"/></font>
										</xsl:when>
		
										<xsl:when test="@status = 'problem'">
											<font style="color:#993300;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:when test="@status = 'error'">
											<font style="color:red;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:otherwise>
											<font><xsl:value-of select="@items"/></font>
										</xsl:otherwise>
										
									</xsl:choose>
							</xsl:if>
							
						</xsl:for-each>
						</td>
						
						<td>
						<xsl:for-each select="nodes">
							
							<xsl:if test="@module = 'new_pafiscan' and @type = 'queue'">
								<xsl:choose>
										<xsl:when test="@status=  'ok'">
											<font style="color:green;"><xsl:value-of select="@items"/></font>
										</xsl:when>
		
										<xsl:when test="@status = 'problem'">
											<font style="color:#993300;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:when test="@status = 'error'">
											<font style="color:red;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:otherwise>
											<font><xsl:value-of select="@items"/></font>
										</xsl:otherwise>
										
									</xsl:choose>
							</xsl:if>
							
						</xsl:for-each>
						</td>
						
						<td>
						<xsl:for-each select="nodes">
							
							<xsl:if test="contains(@module, 'msa') and @type = 'work'">
								<xsl:choose>
										<xsl:when test="@status=  'ok'">
											<font style="color:green;"><xsl:value-of select="@items"/></font>
										</xsl:when>
		
										<xsl:when test="@status = 'problem'">
											<font style="color:#993300;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:when test="@status = 'error'">
											<font style="color:red;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:otherwise>
											<font><xsl:value-of select="@items"/></font>
										</xsl:otherwise>
										
									</xsl:choose>
							</xsl:if>
							
						</xsl:for-each>
						</td>
						
						<td>
						<xsl:for-each select="nodes">
							
							<xsl:if test="contains(@module, 'msa') and @type = 'queue'">
								<xsl:choose>
										<xsl:when test="@status=  'ok'">
											<font style="color:green;"><xsl:value-of select="@items"/></font>
										</xsl:when>
		
										<xsl:when test="@status = 'problem'">
											<font style="color:#993300;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:when test="@status = 'error'">
											<font style="color:red;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:otherwise>
											<font><xsl:value-of select="@items"/></font>
										</xsl:otherwise>
										
									</xsl:choose>
							</xsl:if>
							
						</xsl:for-each>
						</td>
						
						<td>
						<xsl:for-each select="nodes">
							
							<xsl:if test="@module = 'mda' and @type = 'work'">
								<xsl:choose>
										<xsl:when test="@status=  'ok'">
											<font style="color:green;"><xsl:value-of select="@items"/></font>
										</xsl:when>
		
										<xsl:when test="@status = 'problem'">
											<font style="color:#993300;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:when test="@status = 'error'">
											<font style="color:red;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:otherwise>
											<font><xsl:value-of select="@items"/></font>
										</xsl:otherwise>
										
									</xsl:choose>
							</xsl:if>
							
						</xsl:for-each>
						</td>
						
						<td>
						<xsl:for-each select="nodes">
							
							<xsl:if test="@module = 'mda' and @type = 'queue'">
								<xsl:choose>
										<xsl:when test="@status=  'ok'">
											<font style="color:green;"><xsl:value-of select="@items"/></font>
										</xsl:when>
		
										<xsl:when test="@status = 'problem'">
											<font style="color:#993300;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:when test="@status = 'error'">
											<font style="color:red;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:otherwise>
											<font><xsl:value-of select="@items"/></font>
										</xsl:otherwise>
										
									</xsl:choose>
							</xsl:if>
							
						</xsl:for-each>
						</td>
						
						<td>
						<xsl:for-each select="nodes">
							
							<xsl:if test="@module = 'report'">
								<xsl:choose>
										<xsl:when test="@status=  'ok'">
											<font style="color:green;"><xsl:value-of select="@items"/></font>
										</xsl:when>
		
										<xsl:when test="@status = 'problem'">
											<font style="color:#993300;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:when test="@status = 'error'">
											<font style="color:red;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:otherwise>
											<font><xsl:value-of select="@items"/></font>
										</xsl:otherwise>
										
									</xsl:choose>
							</xsl:if>
							
						</xsl:for-each>
						</td>
						
						<td>
						<xsl:for-each select="nodes">
							
							<xsl:if test=" @module = 'end'">
								<xsl:choose>
										<xsl:when test="@status=  'ok'">
											<font style="color:green;"><xsl:value-of select="@items"/></font>
										</xsl:when>
		
										<xsl:when test="@status = 'problem'">
											<font style="color:#993300;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:when test="@status = 'error'">
											<font style="color:red;"><xsl:value-of select="@items"/></font>
										</xsl:when>
									
										<xsl:otherwise>
											<font><xsl:value-of select="@items"/></font>
										</xsl:otherwise>
										
									</xsl:choose>
							</xsl:if>
							
						</xsl:for-each>
						</td>
					</tr>

			</xsl:for-each>
			
			</tbody>
		
		</table>

	</xsl:template>

</xsl:stylesheet>

