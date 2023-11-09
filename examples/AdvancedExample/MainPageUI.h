#define MAIN_TITLE_VIEW_ID 1
#define MAIN_HELLO_VIEW_ID 2

LcdFlashTextView mainTitleView(&lcdI2C, MAIN_TITLE_VIEW_ID, LCD_CENTER_HORIZONTALLY, LCD_AUTO_VERTICALLY);
LcdScrollableTextView mainHelloView(&lcdI2C, MAIN_HELLO_VIEW_ID, LCD_CENTER_HORIZONTALLY, LCD_AUTO_VERTICALLY, 6, "Hello World", true);

LcdPageRow* mainPageRows[] = {
  new LcdPageRow(new LiquidCrystalView*[2]{ &menuCharmapView, &mainTitleView}, 2),
  new LcdPageRow(new LiquidCrystalView*[1]{ &mainHelloView}, 1),
};
LcdPageLayout mainPageLayout = LcdPageLayout(mainPageRows, 2);
LiquidCrystalPage mainPage = LiquidCrystalPage(&lcdI2C, MAIN_PAGE_ID, &mainPageLayout);
