#define GRAPH_TITLE_VIEW_ID 1
#define GRAPH_REFRESH_VIEW_ID 2
#define GRAPH_VIEW_ID 3

byte* getGraphValues() {
  return state.values.graphValues;
}

LcdFlashTextView graphTitleView(&lcdI2C, GRAPH_TITLE_VIEW_ID, LCD_CENTER_HORIZONTALLY, LCD_AUTO_VERTICALLY, false);
LcdCharView graphRefreshView(&lcdI2C, GRAPH_REFRESH_VIEW_ID, LCD_END_HORIZONTALLY, LCD_AUTO_VERTICALLY, '*');
LcdGraphView graphView(&lcdI2C, GRAPH_VIEW_ID, 0, LCD_AUTO_VERTICALLY, 16, 1, 1, 16, getGraphValues, false);

LcdPageRow* graphPageRows[] = {
  new LcdPageRow(new LiquidCrystalView*[3]{ &backCharmapView, &graphTitleView, &graphRefreshView}, 3),
  new LcdPageRow(new LiquidCrystalView*[1]{ &graphView }, 1),
};
LcdPageLayout graphPageLayout = LcdPageLayout(graphPageRows, 2);
LiquidCrystalPage graphPage = LiquidCrystalPage(&lcdI2C, GRAPH_PAGE_ID, &graphPageLayout, false);
