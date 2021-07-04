<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0" xmlns:syscore="http://stnsllethouse.blog.fc2.com/">
<xsl:output method="xml" version="2.0" encoding="UTF-8" indent="yes" />

<xsl:param name="_temp_wep"/>

<xsl:template match="CallTrans">
	<xsl:element name="Event">
		<xsl:apply-templates />
	</xsl:element>
</xsl:template>

<xsl:template match="*">
	<xsl:copy-of select="."/>
</xsl:template>

<xsl:template match="Chara">
	<xsl:element name="Paty">
		<xsl:if test="@army"><xsl:attribute name="army"><xsl:value-of select="@army"/></xsl:attribute></xsl:if>
		<xsl:if test="not(@army)"><xsl:attribute name="army">friend</xsl:attribute></xsl:if>
		<xsl:attribute name="x"><xsl:value-of select="@x"/></xsl:attribute>
		<xsl:attribute name="y"><xsl:value-of select="@y"/></xsl:attribute>
		<xsl:attribute name="dir"><xsl:value-of select="@dir"/></xsl:attribute>
		<xsl:element name="Member">
			<xsl:attribute name="pason"><xsl:value-of select="@name"/></xsl:attribute>
			<xsl:if test="@job"><xsl:attribute name="job"><xsl:value-of select="@job"/></xsl:attribute></xsl:if>
			<xsl:if test="not(@job)"><xsl:attribute name="job">swdman</xsl:attribute></xsl:if>
			<xsl:if test="@wep"><xsl:attribute name="wep"><xsl:value-of select="@wep"/></xsl:attribute></xsl:if>
			<xsl:if test="not(@wep)"><xsl:attribute name="wep"><xsl:value-of select="$_temp_wep"/></xsl:attribute></xsl:if>
		</xsl:element>
	</xsl:element>
</xsl:template>

</xsl:stylesheet>
