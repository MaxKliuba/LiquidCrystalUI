#define PROGRESSBAR_TITLE_VIEW_ID 1
#define PROGRESSBAR_VIEW_ID 2
#define PROGRESSBAR_VALUE_VIEW_ID 3

byte getProgressBarValue() {
  return state.values.progressBarValue;
}

String getProgressBarTextValue() {
  return (String)state.values.progressBarValue;
}

LcdFlashTextView progressBarTitleView(&lcdI2C, PROGRESSBAR_TITLE_VIEW_ID, LCD_CENTER_HORIZONTALLY, LCD_AUTO_VERTICALLY, false);
LcdProgressBarView progressBarView(&lcdI2C, PROGRESSBAR_VIEW_ID, 1, LCD_AUTO_VERTICALLY, 11, 0, 255, getProgressBarValue);
LcdTextValueView progressBarValueView(&lcdI2C, PROGRESSBAR_VALUE_VIEW_ID, 13, LCD_AUTO_VERTICALLY, 3, getProgressBarTextValue, false);

LcdPageRow* progressBarPageRows[] = {
  new LcdPageRow(new LiquidCrystalView*[2]{ &backCharmapView, &progressBarTitleView }, 2),
  new LcdPageRow(new LiquidCrystalView*[2]{ &progressBarView, &progressBarValueView }, 2),
};
LcdPageLayout progressBarPageLayout = LcdPageLayout(progressBarPageRows, 2);
LiquidCrystalPage progressBarPage = LiquidCrystalPage(&lcdI2C, PROGRESSBAR_PAGE_ID, &progressBarPageLayout);
