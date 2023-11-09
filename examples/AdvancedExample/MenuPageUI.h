#define MENU_TITLE_VIEW_ID 1
#define SET_VALUES_MENU_ITEM_VIEW_ID 2
#define PROGRESSBAR_MENU_ITEM_VIEW_ID 3
#define GRAPH_MENU_ITEM_VIEW_ID 4
#define CUSTOM_VIEW_MENU_ITEM_VIEW_ID 5
#define RESET_VALUES_MENU_ITEM_VIEW_ID 6

LcdFlashTextView menuTitleView(&lcdI2C, MENU_TITLE_VIEW_ID, LCD_CENTER_HORIZONTALLY, LCD_AUTO_VERTICALLY, false);
LcdFlashTextView setValuesMenuItemView(&lcdI2C, SET_VALUES_MENU_ITEM_VIEW_ID, 1, LCD_AUTO_VERTICALLY);
LcdFlashTextView progressBarMenuItemView(&lcdI2C, PROGRESSBAR_MENU_ITEM_VIEW_ID, 1, LCD_AUTO_VERTICALLY);
LcdFlashTextView graphMenuItemView(&lcdI2C, GRAPH_MENU_ITEM_VIEW_ID, 1, LCD_AUTO_VERTICALLY);
LcdFlashTextView customViewMenuItemView(&lcdI2C, CUSTOM_VIEW_MENU_ITEM_VIEW_ID, 1, LCD_AUTO_VERTICALLY);
LcdFlashTextView resetValuesMenuItemView(&lcdI2C, RESET_VALUES_MENU_ITEM_VIEW_ID, 1, LCD_AUTO_VERTICALLY);

LcdPageRow* menuPageRows[] = {
  new LcdPageRow(new LiquidCrystalView*[2]{ &backCharmapView, &menuTitleView }, 2),
  new LcdPageRow(new LiquidCrystalView*[1]{ &setValuesMenuItemView }, 1),
  new LcdPageRow(new LiquidCrystalView*[1]{ &progressBarMenuItemView }, 1),
  new LcdPageRow(new LiquidCrystalView*[1]{ &graphMenuItemView }, 1),
  new LcdPageRow(new LiquidCrystalView*[1]{ &customViewMenuItemView }, 1),
  new LcdPageRow(new LiquidCrystalView*[1]{ &resetValuesMenuItemView }, 1),
};
LcdPageLayout menuPageLayout = LcdPageLayout(menuPageRows, 6);
LiquidCrystalPage menuPage = LiquidCrystalPage(&lcdI2C, MENU_PAGE_ID, &menuPageLayout);
