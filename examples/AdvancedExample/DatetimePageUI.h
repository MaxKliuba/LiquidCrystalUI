#define DATETIME_TITLE_VIEW_ID 1
#define TIME_VALUE_VIEW_ID 2
#define DATE_VALUE_VIEW_ID 3

String getTime() {
  return F(__TIME__);
}

String getDate() {
  return F(__DATE__);
}

LcdFlashTextView datetimeTitleView(&lcdI2C, DATETIME_TITLE_VIEW_ID, LCD_CENTER_HORIZONTALLY, LCD_AUTO_VERTICALLY);
LcdTextValueView timeValueView(&lcdI2C, TIME_VALUE_VIEW_ID, LCD_CENTER_HORIZONTALLY, LCD_AUTO_VERTICALLY, 8, getTime);
LcdTextValueView dateValueView(&lcdI2C, DATE_VALUE_VIEW_ID, LCD_CENTER_HORIZONTALLY, LCD_AUTO_VERTICALLY, 11, getDate);

LcdPageRow* datetimePageRows[] = {
  new LcdPageRow(new LiquidCrystalView*[1]{ &datetimeTitleView }, 1),
  new LcdPageRow(new LiquidCrystalView*[1]{ &timeValueView }, 1),
  new LcdPageRow(new LiquidCrystalView*[1]{ &dateValueView }, 1),
};
LcdPageLayout datetimePageLayout = LcdPageLayout(datetimePageRows, 3);
LiquidCrystalPage datetimePage = LiquidCrystalPage(&lcdI2C, DATETIME_PAGE_ID, &datetimePageLayout);
