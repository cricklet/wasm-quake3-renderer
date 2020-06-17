mergeInto(LibraryManager.library, {
  testJS: function() {
    console.warn('JS called from C++')
    testGlobalJS()
  },
});