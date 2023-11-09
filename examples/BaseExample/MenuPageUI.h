#define BACK_CHARMAP_VIEW_ID 0
#define MENU_TITLE_VIEW_ID 1
#define ITEM_VIEW_1_ID 2
#define ITEM_VIEW_2_ID 3
#define ITEM_VIEW_3_ID 4
#define ITEM_VIEW_4_ID 5
#define ITEM_VIEW_5_ID 6
#define ITEM_VIEW_6_ID 7
#define ITEM_VIEW_7_ID 8
#define ITEM_VIEW_8_ID 9
#define ITEM_VIEW_9_ID 10
#define ITEM_VIEW_10_ID 11

LcdBackCharmapView backCharmapView(&lcdI2C, BACK_CHARMAP_VIEW_ID, LCD_START_HORIZONTALLY, LCD_AUTO_VERTICALLY);
LcdFlashTextView menuTitleView(&lcdI2C, MENU_TITLE_VIEW_ID, LCD_CENTER_HORIZONTALLY, LCD_AUTO_VERTICALLY, false);
LcdFlashTextView itemView1(&lcdI2C, ITEM_VIEW_1_ID, 1, LCD_AUTO_VERTICALLY);
LcdFlashTextView itemView2(&lcdI2C, ITEM_VIEW_2_ID, 1, LCD_AUTO_VERTICALLY);
LcdFlashTextView itemView3(&lcdI2C, ITEM_VIEW_3_ID, 1, LCD_AUTO_VERTICALLY);
LcdFlashTextView itemView4(&lcdI2C, ITEM_VIEW_4_ID, 1, LCD_AUTO_VERTICALLY);
LcdFlashTextView itemView5(&lcdI2C, ITEM_VIEW_5_ID, 1, LCD_AUTO_VERTICALLY);
LcdFlashTextView itemView6(&lcdI2C, ITEM_VIEW_6_ID, 1, LCD_AUTO_VERTICALLY);
LcdFlashTextView itemView7(&lcdI2C, ITEM_VIEW_7_ID, 1, LCD_AUTO_VERTICALLY);
LcdFlashTextView itemView8(&lcdI2C, ITEM_VIEW_8_ID, 1, LCD_AUTO_VERTICALLY);
LcdFlashTextView itemView9(&lcdI2C, ITEM_VIEW_9_ID, 1, LCD_AUTO_VERTICALLY);
LcdFlashTextView itemView10(&lcdI2C, ITEM_VIEW_10_ID, 1, LCD_AUTO_VERTICALLY);

LcdPageRow* menuPageRows[] = {
  new LcdPageRow(new LiquidCrystalView*[2]{ &backCharmapView, &menuTitleView }, 2),
  new LcdPageRow(new LiquidCrystalView*[1]{ &itemView1 }, 1),
  new LcdPageRow(new LiquidCrystalView*[1]{ &itemView2 }, 1),
  new LcdPageRow(new LiquidCrystalView*[1]{ &itemView3 }, 1),
  new LcdPageRow(new LiquidCrystalView*[1]{ &itemView4 }, 1),
  new LcdPageRow(new LiquidCrystalView*[1]{ &itemView5 }, 1),
  new LcdPageRow(new LiquidCrystalView*[1]{ &itemView6 }, 1),
  new LcdPageRow(new LiquidCrystalView*[1]{ &itemView7 }, 1),
  new LcdPageRow(new LiquidCrystalView*[1]{ &itemView8 }, 1),
  new LcdPageRow(new LiquidCrystalView*[1]{ &itemView9 }, 1),
  new LcdPageRow(new LiquidCrystalView*[1]{ &itemView10 }, 1),
};
LcdPageLayout menuPageLayout = LcdPageLayout(menuPageRows, 11);
LiquidCrystalPage menuPage = LiquidCrystalPage(&lcdI2C, MENU_PAGE_ID, &menuPageLayout);
