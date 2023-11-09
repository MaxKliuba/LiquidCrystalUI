#define RESET_VALUES_TITLE_VIEW_ID 1
#define RESET_VALUES_OK_VIEW_ID 2

LcdFlashTextView resetValuesTitleView(&lcdI2C, RESET_VALUES_TITLE_VIEW_ID, LCD_CENTER_HORIZONTALLY, LCD_AUTO_VERTICALLY, false);
LcdFlashTextView resetValuesOkView(&lcdI2C, RESET_VALUES_OK_VIEW_ID, LCD_CENTER_HORIZONTALLY, LCD_AUTO_VERTICALLY);

LcdPageRow* resetValuesPageRows[] = {
  new LcdPageRow(new LiquidCrystalView*[2]{ &backCharmapView, &resetValuesTitleView }, 2),
  new LcdPageRow(new LiquidCrystalView*[1]{ &resetValuesOkView }, 1),
};
LcdPageLayout resetValuesPageLayout = LcdPageLayout(resetValuesPageRows, 2);
LiquidCrystalPage resetValuesPage = LiquidCrystalPage(&lcdI2C, RESET_VALUES_PAGE_ID, &resetValuesPageLayout);
