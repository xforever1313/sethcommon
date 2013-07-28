package com.umlet.element.experimental;

import java.awt.Dimension;

import com.baselet.control.Constants.AlignHorizontal;
import com.baselet.control.Constants.AlignVertical;
import com.umlet.element.experimental.helper.XPoints;
import com.umlet.element.experimental.settings.Settings;
import com.umlet.element.experimental.settings.facets.DefaultGlobalFacet.ElementStyleEnum;

public class PropertiesConfig {

	private AlignHorizontal hAlign;
	private boolean hAlignGloballySet = false;
	private AlignVertical vAlign;
	private boolean vAlignGloballySet = false;
	private float yPos = 0;
	private int leftBuffer = 0;
	private int rightBuffer = 0;
	private Settings specificSettings;
	private Dimension gridElementSize;
	private ElementStyleEnum elementStyle;

	public PropertiesConfig(Settings specificSettings) {
		hAlign = specificSettings.getHAlign();
		vAlign = specificSettings.getVAlign();
		this.specificSettings = specificSettings;
	}

	public PropertiesConfig(Settings settings, Dimension realSize) {
		this(settings);
		setGridElementSize(realSize);
	}

	public AlignHorizontal gethAlign() {
		return hAlign;
	}

	public void sethAlign(AlignHorizontal hAlign) {
		if (!hAlignGloballySet) this.hAlign = hAlign;
	}

	public void sethAlignGlobally(AlignHorizontal hAlign) {
		hAlignGloballySet = true;
		this.hAlign = hAlign;
	}

	public void setvAlignGlobally(AlignVertical vAlign) {
		vAlignGloballySet = true;
		this.vAlign = vAlign;
	}

	public void resetAlign() {
		if (!hAlignGloballySet) this.hAlign = specificSettings.getHAlign();
		if (!vAlignGloballySet) this.vAlign = specificSettings.getVAlign();
	}

	public AlignVertical getvAlign() {
		return vAlign;
	}

	public void setvAlign(AlignVertical vAlign) {
		if (!vAlignGloballySet) this.vAlign = vAlign;
	}

	public float getyPos() {
		return yPos;
	}

	public void addToYPos(float inc) {
		yPos += inc;
	}

	private int maxLeftBuffer = 0;
	private int maxRightBuffer = 0;

	public void addToLeftBuffer(int inc) {
		this.leftBuffer += inc;
		if (leftBuffer > maxLeftBuffer) maxLeftBuffer = leftBuffer;
	}

	public void addToRightBuffer(int inc) {
		this.rightBuffer += inc;
		if (rightBuffer > maxRightBuffer) maxRightBuffer = rightBuffer;
	}

	public int getMaxLeftBuffer() {
		return maxLeftBuffer;
	}

	public int getMaxRightBuffer() {
		return maxRightBuffer;
	}

	public void addToBuffer(int inc) {
		addToLeftBuffer(inc);
		addToRightBuffer(inc);
	}

	public Dimension getGridElementSize() {
		return gridElementSize;
	}

	public XPoints getXLimits(float linePos) {
		XPoints xLimits = specificSettings.getXValues(linePos, gridElementSize.height, gridElementSize.width);
		xLimits.addLeft(leftBuffer);
		xLimits.subRight(rightBuffer);
		return xLimits;
	}

	public XPoints getXLimitsForArea(float bottomYPos, float areaHeight) {
		XPoints xLimitsTop = getXLimits(bottomYPos);
		XPoints xLimitsBottom = getXLimits(bottomYPos - areaHeight);
		return xLimitsTop.intersect(xLimitsBottom);
	}

	public float getDividerPos(float f) {
		return getyPos() - f + 2;
	}

	private float maxTextWidth = 0;
	public void calcMaxTextWidth(float width) {
		maxTextWidth = Math.max(maxTextWidth, width);
	}

	public float getMaxTextWidth() {
		return maxTextWidth;
	}

	public void setGridElementSize(Dimension gridElementSize) {
		this.gridElementSize = gridElementSize;
	}

	public ElementStyleEnum getElementStyle() {
		return elementStyle;
	}

	public void setElementStyle(ElementStyleEnum elementStyle) {
		this.elementStyle = elementStyle;
	}

}
