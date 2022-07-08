/*
 * LiquidCrystalUI - Library
 * https://github.com/MaxKliuba/LiquidCrystalUI
 * by MaxClub Technology, 2022
 */

#include <EncButton.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ILiquidCrystal.h>
#include <LiquidCrystal_I2C_Wrapper.h>
#include <LiquidCrystalCharmaps.h>
#include <LiquidCrystalView.h>
#include <LiquidCrystalPage.h>
#include <LiquidCrystalPager.h>

#define LCD_ADDRESS 0x3f // 0x3f or 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2C_Wrapper lcdWrapper = LiquidCrystal_I2C_Wrapper(&lcd, LCD_COLS, LCD_ROWS);

#define MAIN_PAGE_ID 0
#define DATETIME_PAGE_ID 1
#define MENU_PAGE_ID 2
#define VALUES_PAGE_ID 4
#define CUSTOM_VIEW_PAGE_ID 6
#define PROGRESS_BAR_PAGE_ID 7
#define GRAPH_PAGE_ID 8
#define RESET_PAGE_ID 9

#define MENU_BUTTON_ID 0
#define BACK_BUTTON_ID 0

#define MAIN_TITLE_ID 1
#define HELLO_TEXT_VIEW_ID 2

#define DATETIME_TITLE_ID 1
#define TIME_TEXT_VIEW_ID 2
#define DATE_TEXT_VIEW_ID 3

#define MENU_TITLE_ID 1
#define DATETIME_MENU_ITEM_ID 2
#define VALUES_MENU_ITEM_ID 3
#define GRID_MENU_ITEM_ID 4
#define CUSTOM_VIEW_MENU_ITEM_ID 5
#define PROGRESS_BAR_MENU_ITEM_ID 6
#define GRAPH_MENU_ITEM_ID 7
#define RESET_MENU_ITEM_ID 8

#define VALUES_TITLE_ID 1
#define TEXT_VIEW_ID 2
#define SWITCH_ID 3
#define CHECK_BOX_ID 4

#define PROGRESS_BAR_TEXT_VIEW_ID 1
#define PROGRESS_BAR_ID 2

#define GRAPH_ID 1

#define RESET_TEXT_VIEW_ID 1
#define CANCEL_BUTTON_ID 2
#define OK_BUTTON_ID 3

String getFormattedTime();
String getFormattedDate();
String getTextValue();
bool getSwitchValue();
bool getCheckBoxValue();
uint8_t getProgressBarValue();
uint8_t* getGraphValues();

byte textValue = 0;
bool switchValue = false;
bool checkBoxValue = false;
uint8_t graphValues[15] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28};

LcdCharmapOnlyButton menuButton = LcdCharmapOnlyButton::createMenuButton(&lcdWrapper, MENU_BUTTON_ID, 0, 0, true);
LcdCharmapOnlyButton backButton = LcdCharmapOnlyButton::createBackButton(&lcdWrapper, BACK_BUTTON_ID, 0, 0, true);

LcdTitleView mainPageTitle = LcdTitleView(&lcdWrapper, MAIN_TITLE_ID, X_CENTER, 0, true, true, "Main", 4, false, true, FOCUSED_SCROLL_MODE, 500);
//LcdTextView helloTextView = LcdTextView(&lcdWrapper, HELLO_TEXT_VIEW_ID, 1, 0, false, false, "Hello World!", 14, NULL, 0, AUTO_SCROLL_MODE, 500);

LcdTitleView datetimePageTitle = LcdTitleView(&lcdWrapper, DATETIME_TITLE_ID, X_CENTER, 0, true, true, "Date&Time", 9, true, false, FOCUSED_SCROLL_MODE, 500);
//LcdTextView timeTextView = LcdTextView(&lcdWrapper, TIME_TEXT_VIEW_ID, 1, 0, true, false, "", 0, getFormattedTime, 8, AUTO_SCROLL_MODE, 500);
//LcdTextView dateTextView = LcdTextView(&lcdWrapper, DATE_TEXT_VIEW_ID, 1, 0, true, false, "", 0, getFormattedDate, 11, AUTO_SCROLL_MODE, 500);

LcdTitleView menuPageTitle = LcdTitleView(&lcdWrapper, MENU_TITLE_ID, X_CENTER, 0, false, false, "Menu", 4, false, false, AUTO_SCROLL_MODE, 500);
LcdTextView valuesMenuItem = LcdTextView(&lcdWrapper, VALUES_MENU_ITEM_ID, 1, 0, true, false, "Values", 13, NULL, 0, FOCUSED_SCROLL_MODE, 500);
//LcdTextView customViewMenuItem = LcdTextView(&lcdWrapper, CUSTOM_VIEW_MENU_ITEM_ID, 1, 0, true, false, "CustomView", 13, NULL, 0, FOCUSED_SCROLL_MODE, 500);
//LcdTextView progressBarMenuItem = LcdTextView(&lcdWrapper, PROGRESS_BAR_MENU_ITEM_ID, 1, 0, true, false, "ProgressBar", 13, NULL, 0, FOCUSED_SCROLL_MODE, 500);
//LcdTextView graphMenuItem = LcdTextView(&lcdWrapper, GRAPH_MENU_ITEM_ID, 1, 0, true, false, "Graph", 13, NULL, 0, FOCUSED_SCROLL_MODE, 500);
LcdTextView resetMenuItem = LcdTextView(&lcdWrapper, RESET_MENU_ITEM_ID, 1, 0, true, false, "Reset", 14, NULL, 0, FOCUSED_SCROLL_MODE, 500);

LcdTitleView valuesPageTitle = LcdTitleView(&lcdWrapper, VALUES_TITLE_ID, X_CENTER, 0, false, false, "Values", 6, false, false, AUTO_SCROLL_MODE, 500);
LcdTextView textView = LcdTextView(&lcdWrapper, TEXT_VIEW_ID, 1, 0, true, true, "Value:", 6, getTextValue, 3, FOCUSED_SCROLL_MODE, 500);
LcdSwitch switchView = LcdSwitch(&lcdWrapper, SWITCH_ID, 1, 0, true, "Switch ", 7, getSwitchValue, FOCUSED_SCROLL_MODE, 500);
LcdCheckBox checkBox = LcdCheckBox(&lcdWrapper, CHECK_BOX_ID, 1, 0, true, "CheckBox ", 9, getCheckBoxValue, FOCUSED_SCROLL_MODE, 500);

//LcdTextView progressBarTextView = LcdTextView(&lcdWrapper, PROGRESS_BAR_TEXT_VIEW_ID, 7, 0, true, true, "Value:", 6, getTextValue, 3, FOCUSED_SCROLL_MODE, 500);
//LcdProgressBar progressBar = LcdProgressBar(&lcdWrapper, PROGRESS_BAR_ID, 0, 0, false, 16, getProgressBarValue);
//
//LcdGraph graphView = LcdGraph(&lcdWrapper, GRAPH_ID, 1, 0, 15, 2, 0, 32, getGraphValues);

LcdTextView resetTextView = LcdTextView(&lcdWrapper, RESET_TEXT_VIEW_ID, X_CENTER, 0, false, false, "Reset?", 6, NULL, 0, AUTO_SCROLL_MODE, 500);
LcdCharTextButton cancelButton = LcdCharTextButton(&lcdWrapper, CANCEL_BUTTON_ID, 1, 0, true, "CNL", 3, 'x', true, FOCUSED_SCROLL_MODE, 500);
//LcdCharTextButton okButton = LcdCharTextButton(&lcdWrapper, OK_BUTTON_ID, 10, 0, true, "OK", 2, '+', true, FOCUSED_SCROLL_MODE, 500);
LcdCharmapTextButton okButton = LcdCharmapTextButton(&lcdWrapper, OK_BUTTON_ID, 10, 0, true, "OK", 2, _APPLY_CHARMAP_LOCATION, _APPLY_CHARMAP, true, FOCUSED_SCROLL_MODE, 500);

LcdPageRow* mainPageRows[] = {
  new LcdPageRow(new LiquidCrystalView*[2]{&mainPageTitle, &menuButton}, 2),
  //  new LcdPageRow(new LiquidCrystalView*[1]{&helloTextView}, 1),
};
LcdPageLayout mainPageLayout = LcdPageLayout(mainPageRows, 1);
LiquidCrystalPage mainPage = LiquidCrystalPage(&lcdWrapper, MAIN_PAGE_ID, &mainPageLayout);

LcdPageRow* datetimePageRows[] = {
  new LcdPageRow(new LiquidCrystalView*[2]{&datetimePageTitle, &menuButton}, 2),
//  new LcdPageRow(new LiquidCrystalView*[1]{&timeTextView}, 1),
//  new LcdPageRow(new LiquidCrystalView*[1]{&dateTextView}, 1),
};
LcdPageLayout datetimePageLayout = LcdPageLayout(datetimePageRows, 1);
LiquidCrystalPage datetimePage = LiquidCrystalPage(&lcdWrapper, DATETIME_PAGE_ID, &datetimePageLayout);

LcdPageRow* menuPageRows[] = {
  new LcdPageRow(new LiquidCrystalView*[2]{&menuPageTitle, &backButton}, 2),
  new LcdPageRow(new LiquidCrystalView*[1]{&valuesMenuItem}, 1),
//  new LcdPageRow(new LiquidCrystalView*[1]{&customViewMenuItem}, 1),
//  new LcdPageRow(new LiquidCrystalView*[1]{&progressBarMenuItem}, 1),
//  new LcdPageRow(new LiquidCrystalView*[1]{&graphMenuItem}, 1),
  new LcdPageRow(new LiquidCrystalView*[1]{&resetMenuItem}, 1),
};
LcdPageLayout menuPageLayout = LcdPageLayout(menuPageRows, 3);
LiquidCrystalPage menuPage = LiquidCrystalPage(&lcdWrapper, MENU_PAGE_ID, &menuPageLayout);

LcdPageRow* valuesPageRows[] = {
  new LcdPageRow(new LiquidCrystalView*[2]{&backButton, &valuesPageTitle}, 2),
  new LcdPageRow(new LiquidCrystalView*[1]{&textView}, 1),
  new LcdPageRow(new LiquidCrystalView*[1]{&switchView}, 1),
  new LcdPageRow(new LiquidCrystalView*[1]{&checkBox}, 1),
};
LcdPageLayout valuesPageLayout = LcdPageLayout(valuesPageRows, 4);
LiquidCrystalPage valuesPage = LiquidCrystalPage(&lcdWrapper, VALUES_PAGE_ID, &valuesPageLayout);

//LcdPageRow* progressBarPageRows[] = {
//  new LcdPageRow(new LiquidCrystalView*[2]{&backButton, &progressBarTextView}, 2),
//  new LcdPageRow(new LiquidCrystalView*[1]{&progressBar}, 1),
//};
//LcdPageLayout  progressBarPageLayout = LcdPageLayout(progressBarPageRows, 2);
//LiquidCrystalPage  progressBarPage = LiquidCrystalPage(&lcdWrapper, PROGRESS_BAR_PAGE_ID, & progressBarPageLayout);
//
//LcdPageRow* graphPageRows[] = {
//  new LcdPageRow(new LiquidCrystalView*[2]{&backButton, &graphView}, 2),
//};
//LcdPageLayout  graphPageLayout = LcdPageLayout(graphPageRows, 1);
//LiquidCrystalPage graphPage = LiquidCrystalPage(&lcdWrapper, GRAPH_PAGE_ID, & graphPageLayout);

LcdPageRow* resetPageRows[] = {
  new LcdPageRow(new LiquidCrystalView*[2]{&backButton, &resetTextView}, 2),
  new LcdPageRow(new LiquidCrystalView*[2]{&cancelButton, &okButton}, 2),
};
LcdPageLayout resetPageLayout = LcdPageLayout(resetPageRows, 2);
LiquidCrystalPage resetPage = LiquidCrystalPage(&lcdWrapper, RESET_PAGE_ID, &resetPageLayout);

LiquidCrystalPage* pages[] = {
  &mainPage,
  &datetimePage,
  &menuPage,
  &valuesPage,
  //  &progressBarPage,
  //  &graphPage,
  &resetPage
};
LiquidCrystalPager pager = LiquidCrystalPager(pages, 5);

EncButton<EB_TICK, 2, 3, 4> enc(INPUT);

void setup() {
  randomSeed(analogRead(A0));

  lcd.init();
  lcd.backlight();

//  progressBarPage.setDownArrowVisibility(false);
//  graphPage.setDownArrowVisibility(false);
}

void loop() {
  enc.tick();

  pager.tick();

  switch (pager.getCurrentPageId()) {
    case MAIN_PAGE_ID:
      mainPageProcess();
      break;
    case DATETIME_PAGE_ID:
      datetimePageProcess();
      break;
    case MENU_PAGE_ID:
      menuPageProcess();
      break;
    case VALUES_PAGE_ID:
      valuesPageProcess();
      break;
    case PROGRESS_BAR_PAGE_ID:
      progressBarPageProcess();
      break;
    case GRAPH_PAGE_ID:
      graphPageProcess();
      break;
    case RESET_PAGE_ID:
      resetPageProcess();
      break;
  }
}

String getFormattedTime() {
  return F(__TIME__);
}

String getFormattedDate() {
  return F(__DATE__);
}

String getTextValue() {
  return (String)textValue;
}

bool getSwitchValue() {
  return switchValue;
}

bool getCheckBoxValue() {
  return checkBoxValue;
}

uint8_t getProgressBarValue() {
  return textValue;
}

uint8_t* getGraphValues() {
  for (byte i = 0; i < 15; i++) {
    graphValues[i] = random(0, 32);
  }
  return graphValues;
}

void mainPageProcess() {
  if (enc.isTurn()) {
    if (mainPage.getSelectedItemId() == MAIN_TITLE_ID && mainPageTitle.isEditing()) {
      if (enc.isRight()) {
        pager.navigateTo(DATETIME_PAGE_ID);
      }
    } else {
      if (enc.isRight()) {
        mainPage.nextItem();
      } else if (enc.isLeft()) {
        mainPage.prevItem();
      }
    }
  }

  if (enc.hasClicks(1)) {
    switch (mainPage.getSelectedItemId()) {
      case MENU_BUTTON_ID:
        pager.navigateTo(MENU_PAGE_ID);
        break;
      case MAIN_TITLE_ID:
        mainPageTitle.isEditing() ? mainPageTitle.endEditing() : mainPageTitle.beginEditing();
        break;
    }
  }
}

void datetimePageProcess() {
  if (enc.isTurn()) {
    if (datetimePage.getSelectedItemId() == DATETIME_TITLE_ID && datetimePageTitle.isEditing()) {
      if (enc.isLeft()) {
        pager.navigateTo(MAIN_PAGE_ID);
      }
    } else {
      if (enc.isRight()) {
        datetimePage.nextItem();
      } else if (enc.isLeft()) {
        datetimePage.prevItem();
      }
    }
  }

  if (enc.hasClicks(1)) {
    switch (datetimePage.getSelectedItemId()) {
      case MENU_BUTTON_ID:
        pager.navigateTo(MENU_PAGE_ID);
        break;
      case DATETIME_PAGE_ID:
        datetimePageTitle.isEditing() ? datetimePageTitle.endEditing() : datetimePageTitle.beginEditing();
        break;
    }
  }
}

void menuPageProcess() {
  if (enc.isRight()) {
    menuPage.nextItem();
  } else if (enc.isLeft()) {
    menuPage.prevItem();
  }

  if (enc.hasClicks(1)) {
    switch (menuPage.getSelectedItemId()) {
      case BACK_BUTTON_ID:
        pager.navigateTo(MAIN_PAGE_ID);
        break;
      case VALUES_MENU_ITEM_ID:
        pager.navigateTo(VALUES_PAGE_ID);
        break;
      case CUSTOM_VIEW_MENU_ITEM_ID:
        pager.navigateTo(CUSTOM_VIEW_PAGE_ID);
        break;
      case PROGRESS_BAR_MENU_ITEM_ID:
        pager.navigateTo(PROGRESS_BAR_PAGE_ID);
        break;
      case GRAPH_MENU_ITEM_ID:
        pager.navigateTo(GRAPH_PAGE_ID);
        break;
      case RESET_MENU_ITEM_ID:
        pager.navigateTo(RESET_PAGE_ID);
        break;
    }
  }
}

void valuesPageProcess() {
  if (enc.isTurn()) {
    if (valuesPage.getSelectedItemId() == TEXT_VIEW_ID && textView.isEditing()) {
      enc.isLeft() ? textValue-- : textValue++;
      textView.notifyDataChanged();
    } else {
      if (enc.isRight()) {
        valuesPage.nextItem();
      } else if (enc.isLeft()) {
        valuesPage.prevItem();
      }
    }
  }

  if (enc.hasClicks(1)) {
    switch (valuesPage.getSelectedItemId()) {
      case BACK_BUTTON_ID:
        pager.navigateTo(MENU_PAGE_ID);
        break;
      case TEXT_VIEW_ID:
        textView.isEditing() ? textView.endEditing() : textView.beginEditing();
        break;
      case SWITCH_ID:
        switchValue = !switchValue;
        switchView.notifyDataChanged();
        break;
      case CHECK_BOX_ID:
        checkBoxValue = !checkBoxValue;
        checkBox.notifyDataChanged();
        break;
    }
  }
}

void progressBarPageProcess() {
  //  if (enc.isTurn()) {
  //    if (progressBarPage.getSelectedItemId() == PROGRESS_BAR_TEXT_VIEW_ID && progressBarTextView.isEditing()) {
  //      enc.isLeft() ? textValue-- : textValue++;
  //      progressBarTextView.notifyDataChanged();
  //      progressBar.notifyDataChanged();
  //    } else {
  //      if (enc.isRight()) {
  //        progressBarPage.nextItem();
  //      } else if (enc.isLeft()) {
  //        progressBarPage.prevItem();
  //      }
  //    }
  //  }
  //
  //  if (enc.hasClicks(1)) {
  //    switch (progressBarPage.getSelectedItemId()) {
  //      case BACK_BUTTON_ID:
  //        pager.navigateTo(MENU_PAGE_ID);
  //        break;
  //      case PROGRESS_BAR_TEXT_VIEW_ID:
  //        progressBarTextView.isEditing() ? progressBarTextView.endEditing() : progressBarTextView.beginEditing();
  //        break;
  //    }
  //  }
}

void graphPageProcess() {
  //  if (enc.isTurn()) {
  //    graphView.notifyDataChanged();
  //  }
  //
  //  if (enc.hasClicks(1)) {
  //    switch (graphPage.getSelectedItemId()) {
  //      case BACK_BUTTON_ID:
  //        pager.navigateTo(MENU_PAGE_ID);
  //        break;
  //    }
  //  }
}

void resetPageProcess() {
  if (enc.isRight()) {
    resetPage.nextItem();
  } else if (enc.isLeft()) {
    resetPage.prevItem();
  }

  if (enc.hasClicks(1)) {
    switch (resetPage.getSelectedItemId()) {
      case BACK_BUTTON_ID:
        pager.navigateTo(MENU_PAGE_ID);
        break;
      case CANCEL_BUTTON_ID:
        pager.navigateTo(MENU_PAGE_ID);
        break;
      case OK_BUTTON_ID:
        textValue = 0;
        switchValue = false;
        checkBoxValue = false;
        pager.navigateTo(MENU_PAGE_ID);
        break;
    }
  }
}
