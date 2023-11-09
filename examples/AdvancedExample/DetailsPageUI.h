#define DETAILS_TITLE_VIEW_ID 1

LcdFlashTextView detailsTitleView(&lcdI2C, DETAILS_TITLE_VIEW_ID, LCD_CENTER_HORIZONTALLY, LCD_AUTO_VERTICALLY);

LcdPageRow* detailsPageRows[] = {
  new LcdPageRow(new LiquidCrystalView*[1]{ &detailsTitleView }, 1),
};
LcdPageLayout detailsPageLayout = LcdPageLayout(detailsPageRows, 1);
LiquidCrystalPage detailsPage = LiquidCrystalPage(&lcdI2C, DETAILS_PAGE_ID, &detailsPageLayout);
