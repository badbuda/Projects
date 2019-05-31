
// http://stackoverflow.com/questions/3549894/javascript-data-structure-for-fast-lookup-and-ordered-looping
//Create a data structure yourselves. Store the ordering in an array that is internal to the structure. Store the objects mapped by a key in a regular object. Let's call it OrderedMap which will have a map, an array, and four basic methods.
//OrderedMap
//    map
//    _array

//    set(key, value)
//    get(key)
//    remove(key)
//    forEach(fn)

function OrderedMap() {
    this.map = {};
    this._array = [];
}


OrderedMap.prototype.isContainKey = function (key) {
    // key already exists, replace value
    return (key in this.map);
};


OrderedMap.prototype.set = function (key, value) {
    // key already exists, replace value
    if (key in this.map) {
        this.map[key] = value;
    }
        // insert new key and value
    else {
        this._array.push(key);
        this.map[key] = value;
    }
};


OrderedMap.prototype.remove = function (key) {
    var index = this._array.indexOf(key);
    if (index == -1) {
        throw new Error('key does not exist');
    }
    this._array.splice(index, 1);
    delete this.map[key];
};


OrderedMap.prototype.get = function (key) {
    return this.map[key];
};

OrderedMap.prototype.forEach = function (f) {
    var key, value;
    for (var i = 0; i < this._array.length; i++) {
        key = this._array[i];
        value = this.map[key];
        f(key, value);
    }
};


