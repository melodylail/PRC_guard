<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:template match="/mars_monitor">
		<table class="treeTable">
		<thead>
			<tr>
				<th width="30%">Global Mobule</th>
				<th width="70%">Status</th>
			</tr>
		</thead>
		<tbody>
			<tr id="db">
				<td>DataBase</td>
				<td><xsl:value-of select="@db"/></td>
			</tr>						<tr id="njdc-db">				<td>NJDC DB Sync Delay</td>				<td><xsl:value-of select="@db-njdc"/></td>			</tr>						<tr id="iad1-db">				<td>IAD1 DB Sync Delay</td>				<td><xsl:value-of select="@db-iad1"/></td>			</tr>
		</tbody>
		</table>
		<br/>
		<xsl:for-each select="process">
			<xsl:value-of select="@name"/>
			<table class="treeTable">
				<thead>
					<tr>
						<th width="50%">Module</th>
						<th width="10%">Status</th>
						<th width="20%">Type</th>
						<th width="20%">Items</th>
					</tr>
				</thead>
				<tbody>
					<xsl:for-each select="nodes">
					<xsl:variable name="noname">node-<xsl:value-of select="../@name"/>-<xsl:value-of select="@module"/>-<xsl:value-of select="@type"/></xsl:variable>
					<tr id="{$noname}">
						<td><xsl:value-of select="@module"/></td>
						<td><xsl:value-of select="@status"/></td>
						<td><xsl:value-of select="@type"/></td>
						<td><xsl:value-of select="@items"/></td>
					</tr>
						<xsl:for-each select="node">
							<tr class="child-of-{$noname}">
								<td><xsl:value-of select="@dir"/></td>
								<td><xsl:value-of select="@status"/></td>
								<td></td>
								<td><xsl:value-of select="@items"/></td>
							</tr>
						</xsl:for-each>
					</xsl:for-each>
				</tbody>
			</table>
			<br/>
		</xsl:for-each>
	</xsl:template>
</xsl:stylesheet>
