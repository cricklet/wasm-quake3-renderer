export const TEST = 'bundling works';
export function isEmpty(obj) {
    for (var prop in obj) {
        if (obj.hasOwnProperty(prop))
            return false;
    }
    return true;
}
//# sourceMappingURL=helper.js.map