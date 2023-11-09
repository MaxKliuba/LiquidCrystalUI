void processPages() {
  switch (pager.getCurrentPageId()) {
    case MENU_PAGE_ID: menuPageProcess(); break;
    // ,,,
  }
}

void menuPageProcess() {
  if (enc.isTurn()) {
    if (enc.isRight()) {
      menuPage.nextItem();
    } else {
      menuPage.prevItem();
    }
  }

  if (enc.hasClicks(1)) {
    switch (menuPage.getSelectedItemId()) {
      case BACK_CHARMAP_VIEW_ID:
        // pager.navigateTo(PAGE_ID);
        break;
      case ITEM_VIEW_1_ID:
        // ...
        break;
    }
  }
}
